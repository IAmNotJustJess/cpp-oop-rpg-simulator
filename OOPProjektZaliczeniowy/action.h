#pragma once

enum ACTION_TYPE {
	DAMAGE = 0,
	HEAL = 1,
	SHIELD = 2,
	BUFF = 3,
	DEBUFF = 4,
};

enum SUB_TYPE {
	ADDITIVE = 0, // For DMG ACTION_TYPE
	MULTIPLICITIVE = 1
};

enum TARGETTING {
	SINGLE_TARGET = 0,
	BLAST = 1,
	AOE = 2
};

enum SCALING {
	HP = 0,
	DEF = 1,
	ATK = 2
};

class Action {
	ACTION_TYPE type;
	SUB_TYPE subtype;
	TARGETTING targetting;
	double power;
};