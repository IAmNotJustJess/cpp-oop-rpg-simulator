#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "actionList.h"

using namespace std;

class NPC {
private:
	string name;
	string desc;
	double maxHP;
	double hp;
	double def;
	double atk;
	double spd;
	int xp;
	int level;
	int displayHP;
	int displayDEF;
	int displayATK;
	int displaySPD;
	ActionList actionList;
public:
	NPC();
	NPC(string _name, string _desc, double _hp, double _def, double _atk, double _spd, int _xp, int _level);
	void calcDisplay();
};