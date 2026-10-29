#!/usr/bin/env python3
"""Read-only Brisk save compression experiment. Never writes save files.

Uses a GBA BIOS LZ77-compatible stream, not desktop-only zlib. This is a
feasibility tool, not a save validator, migrator, or release serializer.
"""
import argparse
from collections import defaultdict, deque
import hashlib
import json
from pathlib import Path
import random
import struct

SECTOR = 4096
DATA = 3968
SIGNATURE = 0x08012025
PROFILE_CAPACITY = 16 * (SECTOR - 12) - 64


def lz_encode(src):
    """BIOS type 0x10 LZ77: 4 KiB window, 3..18 byte matches."""
    if len(src) >= 1 << 24:
        raise ValueError("LZ77 length exceeds 24-bit header")
    out = bytearray(b'\x10' + len(src).to_bytes(3, 'little'))
    positions = defaultdict(deque)
    pos = 0
    while pos < len(src):
        flag_at = len(out)
        out.append(0)
        flags = 0
        for bit in range(8):
            if pos == len(src):
                break
            key = src[pos:pos + 3]
            candidates = positions[key]
            while candidates and candidates[0] < pos - 4096:
                candidates.popleft()
            length, distance = 0, 0
            if len(key) == 3:
                for prev in reversed(candidates):
                    size = 3
                    while size < 18 and pos + size < len(src) and src[prev + size] == src[pos + size]:
                        size += 1
                    if size > length:
                        length, distance = size, pos - prev
                    if size == 18:
                        break
            if length >= 3:
                flags |= 0x80 >> bit
                token = ((length - 3) << 12) | (distance - 1)
                out.extend(token.to_bytes(2, 'big'))
                used = length
            else:
                out.append(src[pos])
                used = 1
            for at in range(pos, pos + used):
                if at + 3 <= len(src):
                    bucket = positions[src[at:at + 3]]
                    while bucket and bucket[0] < at - 4096:
                        bucket.popleft()
                    bucket.append(at)
            pos += used
        out[flag_at] = flags
    return bytes(out)


def lz_decode(src):
    if len(src) < 4 or src[0] != 0x10:
        raise ValueError("Invalid LZ77 header")
    size = int.from_bytes(src[1:4], 'little')
    out = bytearray()
    at = 4
    while len(out) < size:
        if at >= len(src):
            raise ValueError("Truncated LZ77 flags")
        flags = src[at]
        at += 1
        for bit in range(8):
            if len(out) == size:
                break
            if flags & (0x80 >> bit):
                if at + 2 > len(src):
                    raise ValueError("Truncated LZ77 match")
                token = int.from_bytes(src[at:at + 2], 'big')
                at += 2
                count, distance = (token >> 12) + 3, (token & 4095) + 1
                if distance > len(out) or len(out) + count > size:
                    raise ValueError("Invalid LZ77 match")
                for _ in range(count):
                    out.append(out[-distance])
            else:
                if at >= len(src):
                    raise ValueError("Truncated LZ77 literal")
                out.append(src[at])
                at += 1
    if at != len(src):
        raise ValueError("Unexpected trailing compressed bytes")
    return bytes(out)


def xor_pc(snapshot, box_count=20):
    """Involution over exact BoxPokemon bytes; no field normalization.

    Snapshot sectors 0..13 are in logical ID order, followed by physical
    extension sectors 28..31. BoxPokemon is 80 bytes in the inspected build.
    Preserve all padding, empty records, corrupt checksums, and unused bits.
    This supports the inspected 20-box layout only, not the older 14-box one.
    """
    out = bytearray(snapshot)
    storage_offsets = [i * SECTOR for i in range(5, 18)]
    storage = bytearray().join(out[p:p + DATA] for p in storage_offsets)
    for i in range(box_count * 30):
        start = 4 + i * 80
        personality, ot = struct.unpack_from('<II', storage, start)
        key = personality ^ ot
        for word in range(start + 32, start + 80, 4):
            value, = struct.unpack_from('<I', storage, word)
            struct.pack_into('<I', storage, word, value ^ key)
    for i, p in enumerate(storage_offsets):
        out[p:p + DATA] = storage[i * DATA:(i + 1) * DATA]
    return bytes(out)


def bank_snapshot(save, bank):
    sectors = {}
    counter = None
    for physical in range(bank * 14, (bank + 1) * 14):
        sector = save[physical * SECTOR:(physical + 1) * SECTOR]
        sid, checksum, signature, generation = struct.unpack_from('<HHII', sector, SECTOR - 12)
        if signature != SIGNATURE or sid >= 14 or sid in sectors:
            raise ValueError("Bank has missing, duplicate, or unrecognized sectors")
        if counter is not None and counter != generation:
            raise ValueError("Bank contains mixed save generations")
        counter = generation
        sectors[sid] = sector
    extension = save[28 * SECTOR:32 * SECTOR]
    extension_info = []
    for i in range(4):
        sid, checksum, signature, generation = struct.unpack_from('<HHII', extension, (i + 1) * SECTOR - 12)
        extension_info.append({'sector': 28 + i, 'id': sid,
                               'recognized': signature == SIGNATURE and sid == 0x100 + i,
                               'counter_matches_bank': generation == counter})
    return b''.join(sectors[i] for i in range(14)) + extension, counter, extension_info


def measure(snapshot, transform=False):
    source = xor_pc(snapshot) if transform else snapshot
    encoded = lz_encode(source)
    restored = lz_decode(encoded)
    if transform:
        restored = xor_pc(restored)
    if restored != snapshot:
        raise AssertionError("Compression round trip changed save bytes")
    return {'snapshot_bytes': len(snapshot), 'compressed_bytes': len(encoded),
            'proposed_capacity_bytes': PROFILE_CAPACITY,
            'remaining_bytes': PROFILE_CAPACITY - len(encoded),
            'fits_this_snapshot': len(encoded) <= PROFILE_CAPACITY,
            'byte_exact_round_trip': True}


def self_test():
    rng = random.Random(20261029)
    for size in [0, 1, 2, 3, 7, 8, 9, 17, 18, 19, 4095, 4096, 4097, 73728]:
        for data in [bytes(size), bytes(rng.randrange(256) for _ in range(size)),
                     (b'PokemonBrisk' * (size // 12 + 1))[:size]]:
            assert lz_decode(lz_encode(data)) == data
    for bad in [b'', b'\x10\x01\x00\x00', b'\x10\x03\x00\x00\x80\x00\x00']:
        try:
            lz_decode(bad)
        except ValueError:
            pass
        else:
            raise AssertionError("Malformed LZ stream accepted")
    data = rng.randbytes(18 * SECTOR)
    assert xor_pc(xor_pc(data)) == data
    assert measure(data, True)['byte_exact_round_trip']
    assert not measure(data)['fits_this_snapshot']
    # Synthetic full PC: 600 distinct keys, varied data, mostly zero EVs and
    # ribbons. This is NOT a legal save or a measured real-player result.
    base = bytearray(18 * SECTOR)
    storage = bytearray(13 * DATA)
    for i in range(600):
        start = 4 + i * 80
        struct.pack_into('<II', storage, start, rng.getrandbits(32), rng.getrandbits(32))
        storage[start + 8:start + 32] = rng.randbytes(24)
        storage[start + 32:start + 56] = rng.randbytes(24)
    for i, sector in enumerate(range(5, 18)):
        base[sector * SECTOR:sector * SECTOR + DATA] = storage[i * DATA:(i + 1) * DATA]
    encrypted = xor_pc(bytes(base))
    return {'tests': 'passed', 'synthetic_full_pc': {
        'encrypted_lz77': measure(encrypted),
        'xor_then_lz77': measure(encrypted, True)},
        'arbitrary_full_entropy_snapshot': measure(data)}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('save', nargs='?', type=Path)
    parser.add_argument('--self-test', action='store_true')
    args = parser.parse_args()
    if args.self_test:
        print(json.dumps(self_test(), indent=2))
        return
    if not args.save:
        parser.error('provide a .sav or --self-test')
    data = args.save.read_bytes()
    if len(data) < 128 * 1024:
        parser.error('expected at least 128 KiB; input is not changed')
    report = {'input_sha256': hashlib.sha256(data).hexdigest(),
              'input_bytes': len(data), 'banks': [],
              'limitations': ['Read-only size experiment; NOT checksum validation or migration.',
                              'No worst-case capacity guarantee and no GBA runtime measurement.',
                              'Compare latest coherent bank; shared PC extension may be newer than backup.',
                              'Snapshots include padding/footer bytes; not the proposed final serializer.']}
    for bank in range(2):
        try:
            snapshot, counter, extension = bank_snapshot(data, bank)
            entry = {'bank': bank, 'counter': counter, 'extension': extension,
                     'plain_lz77': measure(snapshot)}
            # Requiring all extension identifiers avoids applying the current
            # storage transform to a legacy 14-box layout or damaged extension.
            if all(e['recognized'] for e in extension):
                entry['xor_then_lz77'] = measure(snapshot, True)
            else:
                entry['xor_then_lz77'] = 'Skipped: current 20-box extension not fully identified'
            report['banks'].append(entry)
        except ValueError as error:
            report['banks'].append({'bank': bank, 'error': str(error)})
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    main()
