"""Portable unit tests; no private player save data required."""
import importlib.util
import os
import random
import struct
import unittest
from pathlib import Path

spec = importlib.util.spec_from_file_location('probe', Path(__file__).with_name('brisk_multisave_probe.py'))
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


def make_sector(section_id, counter, data=None, sb3=None):
    data = data if data is not None else bytes(probe.SECTOR_DATA_BYTES)
    sb3 = sb3 if sb3 is not None else bytes(probe.SECTOR_SB3_BYTES)
    sector = bytearray(4096)
    sector[:3968] = data
    sector[3968:4084] = sb3
    struct.pack_into('<HHII', sector, 4084, section_id, probe.sector_checksum(data), probe.MAGIC, counter)
    return sector


def sample_flash(counter0=11, counter1=12):
    flash = bytearray(b'\xff' * probe.FLASH_BYTES)
    pc = bytearray(13 * 3968)
    for i in range(600):
        pc[4 + 80*i + 19] = 2  # hasSpecies flag; a synthetic fixture, not legal Pokémon data
    for bank, counter in enumerate((counter0, counter1)):
        for idx in range(14):
            section_id = (idx + 2) % 14  # exercise rotated sector IDs
            content = pc[(section_id-5)*3968:(section_id-4)*3968] if section_id >= 5 else bytes(3968)
            flash[(bank*14 + idx)*4096:(bank*14+idx+1)*4096] = make_sector(section_id, counter, content)
    for idx in range(4):
        content = pc[(9+idx)*3968:(10+idx)*3968]
        flash[(28+idx)*4096:(29+idx)*4096] = make_sector(0x100+idx, counter1, content)
    return bytes(flash)


class TwoSlotProbeTests(unittest.TestCase):
    def setUp(self):
        self.save = sample_flash()

    def test_banks_and_20_boxes(self):
        state, info = probe.get_logical_state(self.save + b'extended emulator')
        self.assertEqual(info['active_bank'], 1)
        self.assertEqual(info['pc_occupied'], 600)
        self.assertEqual(info['trailing_metadata_bytes'], len(b'extended emulator'))
        self.assertEqual(len(state), 73048)

    def test_older_counter_and_wraparound(self):
        wrapped = sample_flash(0xffffffff, 0)
        self.assertEqual(probe.get_logical_state(wrapped)[1]['active_bank'], 1)

    def test_bitflip_extension_fails_closed(self):
        damaged = bytearray(self.save)
        damaged[28 * 4096 + 100] ^= 1
        with self.assertRaisesRegex(probe.SaveValidationError, 'extension invalid'):
            probe.get_logical_state(damaged)

    def test_older_bank_fallback_cannot_take_newer_extension(self):
        damaged = bytearray(self.save)
        damaged[14*4096 + 42] ^= 1
        with self.assertRaisesRegex(probe.SaveValidationError, 'extension invalid'):
            probe.get_logical_state(damaged)

    def test_truncated_is_rejected(self):
        with self.assertRaises(probe.SaveValidationError):
            probe.get_logical_state(self.save[:65536])

    def test_roundtrip_and_xor(self):
        state, _ = probe.get_logical_state(self.save)
        other = probe.reversible_box_xor(state)
        self.assertEqual(probe.reversible_box_xor(other), state)
        for payload in (state, b'abcdef' * 5000, random.Random(42).randbytes(12000)):
            self.assertEqual(probe.gba_lz77_decompress(probe.gba_lz77_compress(payload)), payload)

    def test_a_profile_can_be_too_large(self):
        random_state = random.Random(22).randbytes(probe.LOGICAL_BYTES)
        self.assertGreater(len(probe.gba_lz77_compress(random_state)) + 32, probe.FINAL_CAPACITY)

    def test_inspection_does_not_mutate_bytes(self):
        prior = self.save
        probe.audit(prior)
        self.assertEqual(self.save, prior)

    def test_optional_real_save(self):
        path = os.environ.get('BRISK_TEST_SAVE')
        if not path:
            self.skipTest('set BRISK_TEST_SAVE to test an actual 20-box .sav')
        result = probe.audit(Path(path).read_bytes())
        self.assertEqual(result['pc_total'], 600)
        self.assertTrue(result['fits_preserving_legacy_14_sector_staging'])


if __name__ == '__main__':
    unittest.main()
