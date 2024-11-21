#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "action.h"

using namespace std;

class NPC {
private:
	string name;
	string desc;
	double hp;
	double def;
	double atk;
	int displayHP;
	int displayDEF;
	int displayATK;
	vector<Action> listOfActions;
public:
	NPC();
	NPC(string _name, string _desc, double hp, double def, double atk);
	void calcDisplay();
};