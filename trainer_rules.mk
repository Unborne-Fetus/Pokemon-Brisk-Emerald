# party files are run through trainerproc, which is a tool that converts party data to an output file
# matching the current trainer .h formatting

AUTO_GEN_TARGETS += src/data/trainers.h
AUTO_GEN_TARGETS += src/data/brisk_mode_teams.h
AUTO_GEN_TARGETS += src/data/trainers_frlg.h
AUTO_GEN_TARGETS += src/data/battle_partners.h
AUTO_GEN_TARGETS += test/battle/trainer_control.h
AUTO_GEN_TARGETS += test/battle/partner_control.h
AUTO_GEN_TARGETS += src/data/debug_trainers.h

%.h: %.party $(TRAINERPROC)
	$(CPP) $(CPPFLAGS) -traditional-cpp - < $< | $(TRAINERPROC) -o $@ -i $< -

# Generate one compact per-trainer fifth-team differential, avoiding a
# fifth full gTrainers row that would overflow the 32MB GBA ROM.
AUTO_GEN_TARGETS += src/data/brisk_expert_patch.h
src/data/brisk_expert_patch.h: src/data/brisk_mode_teams.party src/data/brisk_kanto_teams.party tools/brisk/build_compact_expert.py FORCE
	@python3 tools/brisk/build_compact_expert.py --write

$(C_BUILDDIR)/battle_setup.o: c_dep += src/data/brisk_expert_patch.h

# Revalidate the ROM's actual trainer table even when a copied build cache or
# checkout gives an old generated header a newer timestamp than its sources.
# Compare contents so unchanged tables do not force data.o to rebuild.
src/data/brisk_mode_teams.h: src/data/brisk_mode_teams.party src/data/brisk_kanto_teams.party $(TRAINERPROC) FORCE
	@set -e; trap 'rm -f $@.tmp' EXIT; \
	$(CPP) $(CPPFLAGS) -traditional-cpp - < $< | $(TRAINERPROC) -o $@.tmp -i $< -; \
	if ! cmp -s $@.tmp $@; then mv $@.tmp $@; fi
