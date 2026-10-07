ArtisanCave_B1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ArtisanCave_B1F_OnTransition
	.byte 0

ArtisanCave_B1F_OnTransition:
	setflag FLAG_LANDMARK_ARTISAN_CAVE
	end


brisk_static_legendary ArtisanCave_B1F_EventScript_MELOETTA, SPECIES_MELOETTA, 40, FLAG_DEFEATED_MELOETTA
