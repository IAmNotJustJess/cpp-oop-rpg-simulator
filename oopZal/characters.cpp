#include <iostream>
#include <cstdlib>
#include "actions.cpp"

using namespace std;

enum CharacterType {
	PLAYABLE,
	NON_PLAYABLE
};

class Character {
private:
	string name;
	string desc;
	CharacterType type;
	bool isAlive;
	int maxhp;
	int hp;
	int atk;
	int def;
	int energy;
	int max_energy;
	int level;
	int xp;
	double lvlUpHPRatio;
	double lvlUpATKRatio;
	double lvlUpDEFRatio;
	vector<Action> actions;
	vector<Status> statuses;
	int oldMaxHP;
	int oldATK;
	int oldDEF;

public:
	Character() {
		name = "Default";
		desc = "Default description.";
		type = NON_PLAYABLE;
		isAlive = false;
		maxhp = 100;
		hp = maxhp;
		atk = 10;
		def = 10;
		energy = 0;
		max_energy = 100;
		level = 1;
		xp = 0;
		lvlUpHPRatio = 1.5;
		lvlUpATKRatio = 1.2;
		lvlUpDEFRatio = 1.1;
		actions = vector<Action>();
		statuses = vector<Status>();
		oldMaxHP = 0;
		oldATK = 0;
		oldDEF = 0;
	}
	Character(string _name, string _desc, CharacterType _type,
		int _hp, int _atk, int _def,
		int _max_energy, int _level, int _xp,
		double _lvlUpHPRatio, double _lvlUpATKRatio, double _lvlUpDEFRatio
	) {
		name = _name;
		desc = _desc;
		type = _type;
		isAlive = false;
		maxhp = _hp;
		hp = maxhp;
		atk = _atk;
		def = _def;
		energy = 0;
		max_energy = _max_energy;
		level = _level;
		xp = _xp;
		lvlUpHPRatio = _lvlUpHPRatio;
		lvlUpATKRatio = _lvlUpATKRatio;
		lvlUpDEFRatio = _lvlUpDEFRatio;
		actions = vector<Action>();
		statuses = vector<Status>();
		oldMaxHP = 0;
		oldATK = 0;
		oldDEF = 0;
	}
	void checkForLevelUp() {
		if (type != PLAYABLE) {
			return;
		}
		int xpNeeded = 175 * level;
		if (xp < xpNeeded) {
			return;
		}
		level += 1;
		xp -= xpNeeded;
		int oldhp = hp;
		int olddef = def;
		int oldatk = atk;
		hp *= lvlUpHPRatio;
		def *= lvlUpDEFRatio;
		atk *= lvlUpATKRatio;
		cout << name << endl;
		cout << "Level Up! " << level - 1 << " -> " << level << endl;
		cout << "-" << endl;
		cout << "HP: " << oldhp << " -> " << hp << endl;
		cout << "ATK: " << oldatk << " -> " << atk << endl;
		cout << "DEF: " << olddef << " -> " << def << endl;
		checkForLevelUp();
	}
	int takeDamage(int damage) {

		damage = damage * (1 - (1 / (def * 0.05 + 1)));

		if (damage <= 0) {
			damage = 1;
		}

		double totalIncomingMultiplier = 0;
		for (int i = 0; i < statuses.size(); i++) {
			Status currentStatus = statuses.at(i);
			switch (currentStatus.type) {
			case ST_INCOMING_DMG_INCREASE:
				damage += currentStatus.value;
				break;
			case ST_INCOMING_DMG_MULTIPLIER:
				totalIncomingMultiplier += currentStatus.multiplier;
				break;
			default:
				break;
			}
		}

		damage *= totalIncomingMultiplier;
		srand((unsigned)time(NULL));
		damage *= 1 + ((rand() % 21) / 100 - 0.1);

		if (damage <= 0) {
			damage = 1;
		}

		hp -= damage;
		if (hp <= 0) {
			hp = 0;
			isAlive = false;
		}
	}
	int heal(int heal) {

		if (!isAlive) {
			return -1;
		}

		double totalIncomingMultiplier = 0;
		for (int i = 0; i < statuses.size(); i++) {
			Status currentStatus = statuses.at(i);
			switch (currentStatus.type) {
			case ST_HEAL_VALUE:
				heal += currentStatus.value;
				break;
			case ST_HEAL_MULTIPLIER:
				totalIncomingMultiplier += currentStatus.multiplier;
				break;
			default:
				break;
			}
		}
		heal *= totalIncomingMultiplier;

		if (heal <= 0) {
			heal = 1;
		}

		hp += heal;
		if (hp >= maxhp) {
			hp = maxhp;
		}

	}
	void checkStatuses() {
		if (oldATK == 0) {
			oldATK = atk;
		}
		if (oldMaxHP == 0) {
			oldMaxHP = maxhp;
		}
		if (oldDEF == 0) {
			oldDEF = def;
		}
		atk = oldATK;
		maxhp = oldMaxHP;
		def = oldDEF;
		if (statuses.size() <= 0) {
			return;
		}
		double totalATKMultiplier = 0;
		double totalMaxHPMultiplier = 0;
		double totalDEFMultiplier = 0;
		for (int i = 0; i < statuses.size(); i++) {
			Status currentStatus = statuses.at(i);
			switch (currentStatus.type) {
				case ST_ATK_VALUE:
					atk += currentStatus.value;
					break;
				case ST_ATK_MULTIPLIER:
					totalATKMultiplier += currentStatus.multiplier;
					break;
				case ST_MAXHP_VALUE:
					maxhp += currentStatus.value;
					break;
				case ST_MAXHP_MULTIPLIER:
					totalMaxHPMultiplier += currentStatus.multiplier;
					break;
				case ST_DEF_VALUE:
					def += currentStatus.value;
					break;
				case ST_DEF_MULTIPLIER:
					totalDEFMultiplier += currentStatus.multiplier;
					break;
				case ST_DAMAGE_OVER_TIME:
					takeDamage(currentStatus.value);
					break;
			}
			statuses.at(i).endIn -= 1;
		}
		atk *= totalATKMultiplier;
		maxhp *= totalMaxHPMultiplier;
		def *= totalDEFMultiplier;
		for (int i = statuses.size() - 1; i >= 0; i--) {
			if (statuses.at(i).endIn <= 0) statuses.erase(statuses.begin() + i);
		}
	}
	void inflictStatus(Status status) {
		statuses.push_back(status);
	}
	void addAction(Action action) {
		actions.push_back(action);
	}
	Action getAction(int i) {
		return actions.at(i);
	}
	void useAction(Action action, vector<Character>& list, int attackIndex) {
		energy += action.energyGain;
		if (energy >= max_energy) {
			energy = max_energy;
		}
		for (int i = 0; i < action.components.size(); i++) {
			ActionComponent current = action.components.at(i);
			int value = 0;
			switch (current.scaling) {
			case SCT_ATK:
				value = atk;
				break;
			case SCT_DEF:
				value = def;
				break;
			case SCT_HP:
				value = maxhp;
				break;
			case SCT_SHIELD:
				value = getShield();
				break;
			}
			value *= current.multiplier;
			switch (current.purpose) {
			case AP_SINGLE_TARGET:
				list.at(attackIndex).takeDamage(value);
				break;
			case AP_BLAST:
				if (attackIndex >= 1) {
					list.at(attackIndex - 1).takeDamage(value);
				}
				list.at(attackIndex - 1).takeDamage(value);
				if (attackIndex < list.size() - 2) {
					list.at(attackIndex + 1).takeDamage(value);
				}
				break;
			case AP_AOE:
				for (int j = 0; j < list.size(); j++) {
					list.at(j).takeDamage(value);
				}
			}
		}
	}
	void readStatusesCount() {
		cout << statuses.size() << endl;
	}
	int getShield() {
		int value = 0;
		for (int i = 0; i < statuses.size(); i++) {
			if (statuses.at(i).type == ST_SHIELDED) value += statuses.at(i).value;
		}
	}
	
};