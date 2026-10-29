#!/usr/bin/env python3
"""Regressions for previously missed stages, aliases and legacy party syntax."""
import unittest
import trainer_legality as rules
from repair_trainer_legality import balanced_stage, repair_mon


class TrainerLegalityTests(unittest.TestCase):
    def test_starters_must_reach_their_evolution_levels(self):
        self.assertEqual(rules.minimum_level('SPECIES_COMBUSKEN'),16)
        self.assertEqual(rules.stage_species('SPECIES_COMBUSKEN',10),'SPECIES_TORCHIC')
        self.assertEqual(rules.stage_species('SPECIES_COMBUSKEN',16),'SPECIES_COMBUSKEN')
        self.assertEqual(rules.stage_species('SPECIES_BLAZIKEN',35),'SPECIES_COMBUSKEN')

    def test_deep_and_custom_evolution_chains(self):
        self.assertEqual(rules.minimum_level('SPECIES_MEDICHAM'),37)
        self.assertEqual(rules.minimum_level('SPECIES_MANDIBUZZ'),54)
        self.assertEqual(rules.minimum_level('SPECIES_TYRANITAR_MEGA'),55)
        self.assertEqual(rules.minimum_level('SPECIES_SHEDINJA'),20)
        self.assertEqual(rules.minimum_level('SPECIES_IRON_MOTH'),59)

    def test_items_are_not_interpreted_as_hundreds_of_levels(self):
        self.assertEqual(rules.minimum_level('SPECIES_MACHAMP'),28)
        self.assertLess(rules.minimum_level('SPECIES_STEELIX'),100)

    def test_aliases_cannot_hide_duplicate_moves(self):
        self.assertEqual(rules.canonical_move('MOVE_SOLARBEAM'),'MOVE_SOLAR_BEAM')

    def test_early_easy_stat_power_uses_brisk_stats(self):
        sp=balanced_stage('SPECIES_ONIX',10,'Easy',1)
        self.assertLessEqual(sum(rules.SPECIES[rules.canonical(sp)]['stats']),400)
        self.assertLessEqual(rules.minimum_level(sp),10)

    def test_legacy_moves_without_spaces_are_replaced_instead_of_appended(self):
        old='SPECIES_DRAGAPULT @ Life Orb\nLevel: 50\n-Dragon Darts\n-Sucker Punch\n-Phantom Force\n-U Turn'
        repaired=repair_mon(old,'Normal','TRAINER_THOMAS',0)
        moves=[line for line in repaired.splitlines() if line.startswith('-')]
        self.assertIn('SPECIES_DRAKLOAK',repaired)
        self.assertLessEqual(len(moves),4)
        self.assertTrue(all(m[2:] in rules.legal_moves('SPECIES_DRAKLOAK',50) for m in moves))

    def test_signature_foul_play_remains_compatible_with_umbreon(self):
        self.assertIn('MOVE_FOUL_PLAY',rules.legal_moves('SPECIES_UMBREON',100))

    def test_future_level_up_moves_do_not_bypass_level_checks(self):
        self.assertNotIn('MOVE_MAKE_IT_RAIN',rules.legal_moves('SPECIES_GHOLDENGO',30))
        self.assertIn('MOVE_MAKE_IT_RAIN',rules.legal_moves('SPECIES_GHOLDENGO',100))

    def test_unknown_species_cannot_silently_default_to_level_one(self):
        with self.assertRaises(ValueError):rules.minimum_level('SPECIES_NOT_A_POKEMON')


if __name__=='__main__':unittest.main()
