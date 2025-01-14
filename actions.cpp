#include <iostream>
#include <vector>

using namespace std;

enum ActionType {
	AT_BASIC = 0,
	AT_SKILL = 1,
	AT_ULTIMATE = 2
};

enum ActionTarget {
	ATG_SELF,
	ATG_ALLY,
	ATG_ENEMY
};

enum ActionAffect {
	AF_SINGLE_TARGET,
	AF_BLAST,
	AF_AOE,
	AF_RANDOM,
};

enum ActionPurpose {
	AP_ATTACK,
	AP_HEAL,
	AP_STATUS
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
	ST_INCOMING_DMG_VALUE,
	ST_INCOMING_DMG_MULTIPLIER,
	ST_OUTGOING_DMG_VALUE,
	ST_OUTGOING_DMG_MULTIPLIER,
	ST_DAMAGE_OVER_TIME,
	ST_HEAL_OVER_TIME,
	ST_INCOMING_HEAL_VALUE,
	ST_INCOMING_HEAL_MULTIPLIER,
	ST_OUTGOING_HEAL_VALUE,
	ST_OUTGOING_HEAL_MULTIPLIER,
	ST_SHIELDED
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

class ActionComponent {
public:
	double multiplier;
	ActionTarget target;
	ActionAffect affect;
	ActionPurpose purpose;
	ScalingType scaling;
	Status status;
	int special;
	ActionComponent(double _multiplier, ActionTarget _target, ActionAffect _affect, ActionPurpose _purpose, ScalingType _scaling, Status _status, int _special) {
		multiplier = _multiplier;
		target = _target;
		affect = _affect;
		purpose = _purpose;
		scaling = _scaling;
		status = _status;
		special = _special;
	};
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
		addComponent(1.0, ATG_ENEMY, AF_SINGLE_TARGET, AP_ATTACK, SCT_ATK, Status(), 0);
	}
	Action(string _name, string _desc, ActionType _type, int _energyGain, double _multiplier, ActionTarget _target, ActionAffect _affect, ActionPurpose _purpose, ScalingType _scaling, Status _status, int _special) {
		name = _name;
		desc = _desc;
		type = _type;
		energyGain = _energyGain;
		addComponent(_multiplier, _target, _affect, _purpose, _scaling, _status, _special);
	}
	void addComponent(double _multiplier, ActionTarget _target, ActionAffect _affect, ActionPurpose _purpose, ScalingType _scaling, Status _status, int _special) {
		components.push_back(ActionComponent(_multiplier, _target, _affect, _purpose, _scaling, _status, _special));
	}
};

class ActionDealt {
public:
	string name;
	ActionPurpose purpose;
	int value;
	ActionDealt(string _name, ActionPurpose _purpose, int _value) {
		name = _name;
		purpose = _purpose;
		value = _value;
	}
};