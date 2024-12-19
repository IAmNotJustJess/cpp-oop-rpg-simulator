#include <iostream>
#include "characters.cpp"

using namespace std;

class CharactersList {
public:
	vector<Character> list;
	CharactersList() {
		list = vector<Character>();
	};
};