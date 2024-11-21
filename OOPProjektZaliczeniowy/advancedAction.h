#pragma once
#include <iostream>
#include <vector>
#include "action.h"

using namespace std;

class AdvancedAction : public Action {
public:
	vector<Action> additionalActions;
	AdvancedAction();
};