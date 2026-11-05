# Kanto Classic outfit: Jessie secondary reward

New outfit ID 3: Red for male player, Leaf for female player. Previous outfit IDs stay unchanged. Full walking, running, biking, surfing, diving, field move, fishing, watering and decorating sprites are provided.

Source: Team Aqua Asset Repo, Pawkkie FRLG-style player sets. Credit Pawkkie and Poffin_Case for underlying art and Pawkkie's conversion/recolor and dive art.

Jessie battle scripts do not yet exist in-game. After awarding the PRIMARY Jessie prize, the future Jessie win script must execute:

    call Common_EventScript_JessieKantoClassicReward

This special unlocks the hidden Kanto Classic outfit (idempotent). Do not call it on a loss or mere interaction. Full end-to-end battle verification awaits Jessie's implementation.
