OrangeStraitEast_MapScripts::
	.byte 0

@ Change primary tileset/layout format with a full field load in open water.
@ The arrival is two tiles clear of the return boundary to avoid warp loops.
OrangeStraitEast_EventScript_CrossStrait::
	lockall
	warp MAP_ORANGE_STRAIT_WEST, 37, 10
	waitstate
	releaseall
	end
