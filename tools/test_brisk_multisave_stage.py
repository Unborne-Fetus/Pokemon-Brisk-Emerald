"""Unit tests for in-memory migration staging; no user save is embedded."""
import unittest
import random
from brisk_multisave_stage import (stage_in_memory, read_staged_profile, make_profile,
                                  STAGING_CAPACITY, SaveValidationError)
from brisk_multisave_probe import get_logical_state, LOGICAL_BYTES
from test_brisk_multisave_probe import sample_flash


class StagingTests(unittest.TestCase):
    def test_protect_active_and_extension(self):
        old = sample_flash() + b'kept emulator trailer'
        staged, stats = stage_in_memory(old)
        self.assertEqual(stats['stage_bank'], 0)
        self.assertEqual(staged[14*4096:], old[14*4096:])
        self.assertEqual(read_staged_profile(staged, 0), get_logical_state(old)[0])

    def test_stage_works_when_bank_zero_is_newer(self):
        original = bytearray(sample_flash(counter0=13, counter1=12))
        for i in range(4):
            a = 4096*(28+i)
            chunk = bytearray(original[a:a+4096]); chunk[4092:4096] = (13).to_bytes(4, 'little')
            original[a:a+4096] = chunk
        staged, info = stage_in_memory(bytes(original))
        self.assertEqual(info['stage_bank'], 1)
        self.assertEqual(staged[:14*4096], original[:14*4096])

    def test_staged_corruption_rejected(self):
        staged, _ = stage_in_memory(sample_flash())
        damaged = bytearray(staged)
        damaged[100] ^= 1
        with self.assertRaisesRegex(SaveValidationError, 'bad stage sector'):
            read_staged_profile(damaged, 0)

    def test_uncompressible_overflow_rejected(self):
        raw = random.Random(789).randbytes(LOGICAL_BYTES)
        with self.assertRaisesRegex(SaveValidationError, 'cannot fit'):
            make_profile(raw, 15)


if __name__ == '__main__':
    unittest.main()
