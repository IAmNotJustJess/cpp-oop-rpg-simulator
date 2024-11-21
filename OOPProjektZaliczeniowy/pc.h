#pragma once
#include "npc.h"

enum CHARACTER_CLASSES {
	SINGLE_TARGET_ATTACKER = 0,
	MULTI_TARGET_ATTACKER = 1,
	AOE_ATTACKER = 2,
	BUFFER = 3,
	SHIELDER = 4,
	DEBUFFER = 5
};

class PC : public NPC {
	
};