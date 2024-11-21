#pragma once
#include <vector>
#include "action.h"

using namespace std;

class ActionList {
private:
	vector<Action> list;
public:
	int maxWeight;
	Action getSpecificAction();
	Action getRandomAction();
};