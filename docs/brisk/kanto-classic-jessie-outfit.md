# Kanto Classic outfit: Jessie secondary reward

New outfit ID 3: Red for male player, Leaf for female player. Previous outfit IDs stay unchanged. Full walking, running, biking, surfing, diving, field move, fishing, watering and decorating sprites are provided.

Source: Team Aqua Asset Repo, Pawkkie FRLG-style player sets. Credit Pawkkie and Poffin_Case for underlying art and Pawkkie's conversion/recolor and dive art.

Jessie's Rocket Hideout B4F doubles script now awards this outfit and a level-70 Magearna after victory. The outfit uses the existing gender-dependent Kanto Classic visuals. The reward chain executes:

    call Common_EventScript_JessieKantoClassicReward

The outfit unlock is idempotent. Magearna is awarded once, tracked independently of the existing New Mauville static Magearna. When party and PC are full, the player can return to Jessie to collect the outstanding reward without another fight. The scripted rewards still require ROM build and emulator verification. The requested synchronized Aegislash/Archaludon final send-out is not yet guaranteed by ordinary trainer roster order.
