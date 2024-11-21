#pragma once
#include "advancedAction.h"

enum ACTION_COST {
	BASIC = 0,
	SKILL = 1,
	ULTIMATE = 2
};

class PlayerAction : public AdvancedAction {
public:
	ACTION_COST cost;
	string text;
	PlayerAction(ACTION_TYPE _type, SUB_TYPE _subtype, TARGETTING _targetting, SCALING _scale, ELEMENT_TYPE _elementType, double _power, ACTION_COST _cost);
	PlayerAction(AdvancedAction _action, ACTION_COST _cost);
	PlayerAction();
};