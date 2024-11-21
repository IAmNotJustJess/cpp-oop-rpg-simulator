#pragma once

enum ACTION_TYPE {
	DAMAGE = 0,
	HEAL = 1,
	SHIELD = 2,
	BUFF = 3,
	DEBUFF = 4,
};

enum SUB_TYPE {
	ADDITIVE = 0, // For DAMAGE, HEAL, SHIELD ACTION_TYPEs
	MULTIPLICATIVE = 1,
	ATK_INCREASE = 2, // For BUFF and DEBUFF ACTION_TYPEs
	ATK_MULTIPLICATION = 3,
	DEF_INCREASE = 4,
	DEF_MULTIPLICATION = 5,
	HP_INCREASE = 6,
	HP_MULTIPLICATION = 7,
	SPD_INCREASE = 8,
	SPD_MULTIPLICATION = 9,
	ATK_DECREASE = 10,
	DEF_DECREASE = 11,
	SPD_DECREASE = 12,
	HEAL_POWER_MULTIPLICATION = 13,
	SHIELD_POWER_MULTIPLICATION = 14
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

enum ELEMENT_TYPE {
	FIRE = 0,
	EARTH = 1,
	WATER = 2,
	AIR = 3,
	PLASMA = 4,
};

class Action {
public:
	ACTION_TYPE type;
	SUB_TYPE subtype;
	TARGETTING targetting;
	SCALING scaling;
	ELEMENT_TYPE elementType;
	double power;
	Action();
	Action(ACTION_TYPE _type, SUB_TYPE _subtype, TARGETTING _targetting, SCALING _scale, ELEMENT_TYPE _elementType, double power);
};