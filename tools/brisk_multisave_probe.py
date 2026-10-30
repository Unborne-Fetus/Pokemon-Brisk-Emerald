#!/usr/bin/env python3
"""Read-only Brisk Emerald 20-box save validator and compact-slot feasibility probe.

This is research tooling, not a playable dual-profile ROM save converter.
Never modifies the input. Supports optional emulator metadata past 128 KiB.
"""
from __future__ import annotations

import argparse
import collections
import json
import struct
import sys
import zlib
from pathlib import Path

FLASH_BYTES = 128 * 1024
SECTOR_BYTES = 4096
SECTOR_DATA_BYTES = 3968
SECTOR_SB3_BYTES = 116
SECTORS_PER_BANK = 14
EXTRA_START = 28
MAGIC = 0x08012025
BOX_COUNT = 20
MONS_PER_BOX = 30
MON_BYTES = 80
LOGICAL_BYTES = (1 + 4 + 13) * SECTOR_DATA_BYTES + SECTORS_PER_BANK * SECTOR_SB3_BYTES
STAGING_CAPACITY = SECTORS_PER_BANK * SECTOR_DATA_BYTES - 32
FINAL_CAPACITY = 16 * SECTOR_DATA_BYTES - 32


class SaveValidationError(ValueError):
    """Reject incomplete, invalid, or unsupported save layouts."""


def sector_checksum(data: bytes) -> int:
    if len(data) % 4:
        raise ValueError('checksum bytes must be word-aligned')
    words = struct.iter_unpack('<I', data)
    total = sum(word[0] for word in words) & 0xffffffff
    return ((total >> 16) + total) & 0xffff


def read_sector(flash: bytes, index: int) -> dict:
    block = flash[index * SECTOR_BYTES:(index + 1) * SECTOR_BYTES]
    section_id, checksum, signature, counter = struct.unpack_from('<HHII', block, 4084)
    return {'physical': index, 'id': section_id, 'checksum': checksum,
            'signature': signature, 'counter': counter, 'data': block[:SECTOR_DATA_BYTES],
            'sb3': block[SECTOR_DATA_BYTES:4084]}


def validated_banks(flash: bytes) -> list[dict]:
    result = []
    for bank in range(2):
        sec = [read_sector(flash, bank * SECTORS_PER_BANK + i)
               for i in range(SECTORS_PER_BANK)]
        ids = {entry['id'] for entry in sec}
        counters = {entry['counter'] for entry in sec}
        errors = []
        if ids != set(range(SECTORS_PER_BANK)):
            errors.append('missing or duplicate sector IDs')
        if len(counters) != 1:
            errors.append('mixed save counters')
        for s in sec:
            if s['signature'] != MAGIC:
                errors.append(f"invalid signature at sector {s['physical']}")
            if sector_checksum(s['data']) != s['checksum']:
                errors.append(f"invalid checksum at sector {s['physical']}")
        if not errors:
            result.append({'bank': bank, 'counter': sec[0]['counter'],
                           'sections': {s['id']: s for s in sec}})
    return result


def active_bank(valid: list[dict]) -> dict:
    if not valid:
        raise SaveValidationError('neither bank has 14 valid matching sections')
    if len(valid) == 1:
        return valid[0]
    a, b = valid
    if a['counter'] == b['counter']:
        raise SaveValidationError('ambiguous equal counters: refusing to guess')
    # Modulo-aware most recent counter, including 0xFFFFFFFF -> 0 wrap.
    return a if 0 < ((a['counter'] - b['counter']) & 0xffffffff) < 0x80000000 else b


def get_logical_state(file_bytes: bytes) -> tuple[bytes, dict]:
    if len(file_bytes) < FLASH_BYTES:
        raise SaveValidationError(f'truncated flash: {len(file_bytes)} bytes')
    flash = file_bytes[:FLASH_BYTES]
    valid = validated_banks(flash)
    current = active_bank(valid)
    sections = current['sections']
    extension = []
    for sector_id in range(4):
        s = read_sector(flash, EXTRA_START + sector_id)
        if (s['id'] != 0x100 + sector_id or s['signature'] != MAGIC
                or s['counter'] != current['counter']
                or sector_checksum(s['data']) != s['checksum']):
            raise SaveValidationError(f'20-box extension invalid at sector {EXTRA_START + sector_id}')
        extension.append(s['data'])
    sb2 = sections[0]['data']
    sb1 = b''.join(sections[i]['data'] for i in range(1, 5))
    pc = b''.join(sections[i]['data'] for i in range(5, 14)) + b''.join(extension)
    sb3 = b''.join(sections[i]['sb3'] for i in range(14))
    result = sb2 + sb1 + pc + sb3
    assert len(result) == LOGICAL_BYTES
    offset = 4  # BoxPokemon starts 4-byte aligned after currentBox
    occupied = sum(bool(pc[offset + i * MON_BYTES + 19] & 2)
                   for i in range(BOX_COUNT * MONS_PER_BOX))
    info = {'active_bank': current['bank'], 'save_counter': current['counter'],
            'valid_banks': [b['bank'] for b in valid], 'pc_occupied': occupied,
            'pc_total': BOX_COUNT * MONS_PER_BOX,
            'trailing_metadata_bytes': len(file_bytes) - FLASH_BYTES,
            'source_sha256': __import__('hashlib').sha256(file_bytes).hexdigest()}
    return result, info


def reversible_box_xor(state: bytes) -> bytes:
    """XOR all 600 BoxPokemon secure payloads, identical operation to undo."""
    out = bytearray(state)
    pc_offset = 5 * SECTOR_DATA_BYTES
    for index in range(BOX_COUNT * MONS_PER_BOX):
        start = pc_offset + 4 + index * MON_BYTES
        personality, ot_id = struct.unpack_from('<II', out, start)
        key = personality ^ ot_id
        for offset in range(32, 80, 4):
            value = struct.unpack_from('<I', out, start + offset)[0] ^ key
            struct.pack_into('<I', out, start + offset, value)
    return bytes(out)


def gba_lz77_compress(data: bytes) -> bytes:
    """GBA BIOS type 0x10, greedy 4 KiB window / 3..18 match compressor."""
    if len(data) >= 1 << 24:
        raise ValueError('GBA LZ77 has a 24-bit length field')
    out = bytearray(b'\x10' + len(data).to_bytes(3, 'little'))
    positions = collections.defaultdict(collections.deque)
    cursor = 0
    def add_index(at: int) -> None:
        if at + 3 <= len(data):
            key = data[at:at + 3]
            queue = positions[key]
            queue.append(at)
            while queue and at - queue[0] > 4096:
                queue.popleft()
            if len(queue) > 64:
                queue.popleft()
    while cursor < len(data):
        flag_at = len(out)
        out.append(0)
        for bit in range(7, -1, -1):
            if cursor >= len(data):
                break
            best_len, best_distance = 0, 0
            if cursor + 3 <= len(data):
                for previous in reversed(positions.get(data[cursor:cursor + 3], ())):
                    distance = cursor - previous
                    if distance > 4096:
                        break
                    max_len = min(18, len(data) - cursor)
                    match_len = 3
                    while match_len < max_len and data[previous + match_len] == data[cursor + match_len]:
                        match_len += 1
                    if match_len > best_len:
                        best_len, best_distance = match_len, distance
                        if match_len == max_len:
                            break
            if best_len >= 3:
                out[flag_at] |= (1 << bit)
                distance = best_distance - 1
                out.extend((((best_len - 3) << 4) | (distance >> 8), distance & 0xff))
                advance = best_len
            else:
                out.append(data[cursor])
                advance = 1
            for new_at in range(cursor, cursor + advance):
                add_index(new_at)
            cursor += advance
    while len(out) % 4:
        out.append(0)
    return bytes(out)


def gba_lz77_decompress(packed: bytes) -> bytes:
    if not packed or packed[0] != 0x10:
        raise SaveValidationError('not GBA BIOS LZ77 data')
    wanted = int.from_bytes(packed[1:4], 'little')
    out = bytearray()
    pos = 4
    while len(out) < wanted:
        if pos >= len(packed):
            raise SaveValidationError('truncated LZ77 flags')
        flags = packed[pos]
        pos += 1
        for bit in range(7, -1, -1):
            if len(out) >= wanted:
                break
            if flags & (1 << bit):
                if pos + 2 > len(packed):
                    raise SaveValidationError('truncated LZ77 match')
                hi, lo = packed[pos:pos+2]
                pos += 2
                length, distance = (hi >> 4) + 3, ((hi & 0xf) << 8 | lo) + 1
                if distance > len(out) or len(out) + length > wanted:
                    raise SaveValidationError('invalid LZ77 backreference')
                for _ in range(length):
                    out.append(out[-distance])
            else:
                if pos >= len(packed):
                    raise SaveValidationError('truncated LZ77 literal')
                out.append(packed[pos])
                pos += 1
    return bytes(out)


def audit(file_bytes: bytes) -> dict:
    state, info = get_logical_state(file_bytes)
    original = gba_lz77_compress(state)
    transformed_state = reversible_box_xor(state)
    assert reversible_box_xor(transformed_state) == state
    transformed = gba_lz77_compress(transformed_state)
    assert gba_lz77_decompress(original) == state
    assert gba_lz77_decompress(transformed) == transformed_state
    selected = min((('raw', original), ('xor-normalized', transformed)), key=lambda x: len(x[1]))
    size = len(selected[1]) + 32
    return {**info, 'logical_state_bytes': len(state), 'compression': selected[0],
            'gba_lz77_bytes': len(selected[1]), 'packed_plus_header_bytes': size,
            'migration_staging_capacity_bytes': STAGING_CAPACITY,
            'per_profile_capacity_16_sectors_bytes': FINAL_CAPACITY,
            'fits_preserving_legacy_14_sector_staging': size <= STAGING_CAPACITY,
            'fits_proposed_16_sector_profile': size <= FINAL_CAPACITY,
            'tested': 'read-only reversible packing; NOT an in-game two-slot system'}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('save', type=Path)
    args = parser.parse_args()
    try:
        print(json.dumps(audit(args.save.read_bytes()), indent=2))
    except SaveValidationError as exc:
        print(f'SAVE REJECTED: {exc}', file=sys.stderr)
        raise SystemExit(2) from exc


if __name__ == '__main__':
    main()
