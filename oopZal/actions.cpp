#include <iostream>
#include <vector>

using namespace std;

enum ActionType {
	AT_BASIC = 0,
	AT_SKILL = 1,
	AT_ULTIMATE = 2
};

enum ActionPurpose {
	AP_SINGLE_TARGET,
	AP_BLAST,
	AP_AOE,
	AP_RANDOM,
};

enum ScalingType {
	SCT_HP,
	SCT_ATK,
	SCT_DEF,
	SCT_SHIELD
};

enum StatusType {
	ST_NONE,
	ST_MAXHP_VALUE,
	ST_MAXHP_MULTIPLIER,
	ST_ATK_VALUE,
	ST_ATK_MULTIPLIER,
	ST_DEF_VALUE,
	ST_DEF_MULTIPLIER,
	ST_INCOMING_DMG_INCREASE,
	ST_INCOMING_DMG_MULTIPLIER,
	ST_DAMAGE_OVER_TIME,
	ST_HEAL_OVER_TIME,
	ST_HEAL_VALUE,
	ST_HEAL_MULTIPLIER,
	ST_SHIELDED
};

class Action {
public:
	string name;
	string desc;
	ActionType type;
	int energyGain;
	vector<ActionComponent> components;
	Action() {
		name = "Default.";
		desc = "Default description.";
		type = AT_BASIC;
		energyGain = 10;
		addModifier(1.0, AP_ATTACK_SINGLE_TARGET, SCT_ATK, Status(), 0);
	}
	Action(string _name, string _desc, ActionType _type, int _energyGain, double _multiplier, ActionPurpose _purpose, ScalingType _scaling, Status _status, int _special) {
		name = _name;
		desc = _desc;
		type = _type;
		energyGain = _energyGain;
		addModifier(_multiplier, _purpose, _scaling, _status, _special);
	}
	void addModifier(double _multiplier, ActionPurpose _purpose, ScalingType _scaling, Status _status, int _special) {
		components.push_back(ActionComponent(_multiplier, _purpose, _scaling, _status, _special));
	}
};

class ActionComponent {
public:
	double multiplier;
	ActionPurpose purpose;
	ScalingType scaling;
	Status status;
	int special;
	ActionComponent(double _multiplier, ActionPurpose _purpose, ScalingType _scaling, Status _status, int _special) {
		multiplier = _multiplier;
		purpose = _purpose;
		scaling = _scaling;
		status = _status;
		special = _special;
	};
};

class Status {
public:
	int value;
	double multiplier;
	StatusType type;
	int endIn;
	Status() {
		value = 0;
		multiplier = 0.0;
		type = ST_NONE;
		endIn = 0;
	}
	Status(int _value, double _multiplier, StatusType _type, int _endIn) {
		value = _value;
		multiplier = _multiplier;
		type = _type;
		endIn = _endIn;
	}
};