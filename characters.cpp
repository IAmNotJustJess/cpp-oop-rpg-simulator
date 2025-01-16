#include <iostream>
#include <cstdlib>
#include <random>
#include "actions.cpp"

using namespace std;

enum CharacterType {
	PLAYABLE,
	NON_PLAYABLE
};

class Character {
private:
	double lvlUpHPRatio;
	double lvlUpATKRatio;
	double lvlUpDEFRatio;
	vector<Action> actions;
	vector<Status> statuses;
	int oldMaxHP;
	int oldATK;
	int oldDEF;

public:
	string name;
	string desc;
	CharacterType type;
	bool isAlive;
	int maxhp;
	int hp;
	int atk;
	int def;
	int energy;
	int maxEnergy;
	int level;
	int xp;
	Character() {
		name = "Default";
		desc = "Default description.";
		type = NON_PLAYABLE;
		isAlive = true;
		maxhp = 100;
		hp = maxhp;
		atk = 10;
		def = 10;
		energy = 0;
		maxEnergy = 100;
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
		int _maxEnergy, int _level, int _xp,
		double _lvlUpHPRatio, double _lvlUpATKRatio, double _lvlUpDEFRatio
	) {
		name = _name;
		desc = _desc;
		type = _type;
		isAlive = true;
		maxhp = _hp;
		hp = maxhp;
		atk = _atk;
		def = _def;
		energy = 0;
		maxEnergy = _maxEnergy;
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
		if (type != PLAYABLE) return;

		int xpNeeded = 175 * level;

		if (xp < xpNeeded) return;

		level += 1;
		xp -= xpNeeded;
		int hpHolder = hp;
		int defHolder = def;
		int atkHolder = atk;
		hp *= lvlUpHPRatio;
		def *= lvlUpDEFRatio;
		atk *= lvlUpATKRatio;

		cout << endl << name << endl;
		cout << "Poziom w Gore! " << level - 1 << " -> " << level << endl;
		cout << "-" << endl;
		cout << "Punkty Zdrowia: " << hpHolder << " -> " << hp << endl;
		cout << "Atak: " << atkHolder << " -> " << atk << endl;
		cout << "Obrona: " << defHolder << " -> " << def << endl << endl;
		
		checkForLevelUp();
	}
	void clearBeforeBattle() {
		if(oldATK != 0) {
			atk = oldATK;
		}
		if(oldDEF != 0) {
			def = oldDEF;
		}
		if(oldMaxHP != 0) {
			maxhp = oldMaxHP;
		}
		oldATK = 0;
		oldDEF = 0;
		oldMaxHP = 0;
	}
	void scaleToLevel(int toLvl) {
		if(toLvl <= level) return;
		for (int i = level; i < toLvl; i++) {
			hp *= lvlUpHPRatio;
			def *= lvlUpDEFRatio;
			atk *= lvlUpATKRatio;
			level += 1;
			xp *= 1.2;
		}
	}
	void addEnergy(int en) {
		energy += en;
		if(energy >= maxEnergy) {
			energy = maxEnergy;
		}
	}
	int takeDamage(int dmg) {
		
		double damage = dmg;
		damage = damage / (def * 0.005 + 1);

		if (damage < 1) {
			damage = 1;
		}

		double totalIncomingMultiplier = 1;
		for (int i = 0; i < statuses.size(); i++) {
			Status currentStatus = statuses.at(i);
			switch (currentStatus.type) {
			case ST_INCOMING_DMG_VALUE:
				damage += currentStatus.value;
				break;
			case ST_INCOMING_DMG_MULTIPLIER:
				totalIncomingMultiplier *= currentStatus.multiplier;
				break;
			default:
				break;
			}
		}

		damage *= totalIncomingMultiplier;
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dis(0, 21);
		damage *= 1 + ((dis(gen) % 21) / 100 - 0.1);

		if (damage <= 0) {
			damage = 1;
		}

		int damageHolder = damage;
		int shield = getShield();
		if (shield > 0) {
			for (int i = 0; i < statuses.size(); i++) {
				if (statuses.at(i).type == ST_SHIELDED && damage > 0) {
					int difference = statuses.at(i).value - damage;
					if (difference <= 0) {
						damage -= statuses.at(i).value;
						statuses.at(i).value = 0;
						statuses.at(i).endIn = 0;
					}
					else {
						damage = 0;
						statuses.at(i).value = difference;
					}
				}
			}
		}

		if (damage == 0) {
			return damageHolder;
		}

		hp -= damage;
		if (hp <= 0) {
			hp = 0;
			isAlive = false;
		}
		return damageHolder;
	}
	int heal(int heal) {

		if (!isAlive) {
			return -1;
		}

		double totalIncomingMultiplier = 1;
		for (int i = 0; i < statuses.size(); i++) {
			Status currentStatus = statuses.at(i);
			switch (currentStatus.type) {
			case ST_INCOMING_HEAL_VALUE:
				heal += currentStatus.value;
				break;
			case ST_INCOMING_HEAL_MULTIPLIER:
				totalIncomingMultiplier *= currentStatus.multiplier;
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
		return heal;
	}
	void checkStatuses(bool newTurn) {

		if (oldATK == 0) oldATK = atk;
		if (oldMaxHP == 0) oldMaxHP = maxhp;
		if (oldDEF == 0) oldDEF = def;

		atk = oldATK;
		maxhp = oldMaxHP;
		def = oldDEF;
		if (statuses.size() <= 0) return;

		double totalATKMultiplier = 1;
		double totalMaxHPMultiplier = 1;
		double totalDEFMultiplier = 1;

		for (int i = 0; i < statuses.size(); i++) {

			Status currentStatus = statuses.at(i);
			switch (currentStatus.type) {
			case ST_ATK_VALUE:
				atk += currentStatus.value;
				break;
			case ST_ATK_MULTIPLIER:
				totalATKMultiplier *= currentStatus.multiplier;
				break;
			case ST_MAXHP_VALUE:
				maxhp += currentStatus.value;
				break;
			case ST_MAXHP_MULTIPLIER:
				totalMaxHPMultiplier *= currentStatus.multiplier;
				break;
			case ST_DEF_VALUE:
				def += currentStatus.value;
				break;
			case ST_DEF_MULTIPLIER:
				totalDEFMultiplier *= currentStatus.multiplier;
				break;
			case ST_DAMAGE_OVER_TIME:
				if(newTurn) takeDamage(currentStatus.value * currentStatus.multiplier);
				break;
			case ST_HEAL_OVER_TIME:
				if(newTurn) heal(currentStatus.value * currentStatus.multiplier);
				break;
			}
		}
		
		atk *= totalATKMultiplier;
		maxhp *= totalMaxHPMultiplier;
		def *= totalDEFMultiplier;

		if(newTurn) {
			for (int i = statuses.size() - 1; i >= 0; i--) {
				if (statuses.at(i).endIn < 0) statuses.erase(statuses.begin() + i);
			}
		}
	}
	void inflictStatus(Status status, int info) {
		bool found = false;
		switch (status.type) {
		default:
			found = false;
			for(int i = 0; i < statuses.size(); i++) {
				Status& st = statuses.at(i);
				if(st.multiplier == status.multiplier && st.type == status.type && status.value == st.value) {
					st.endIn = status.endIn;
					found = true;
					break;
				}
			}
			if(!found) {
				statuses.push_back(status);
			}
			break;
		case ST_SHIELDED:
			found = false;
			status.value = info;
			for(int i = 0; i < statuses.size(); i++) {
				Status& st = statuses.at(i);
				if(st.value <= status.value && st.multiplier == status.multiplier) {
					st.value = status.value;
					st.endIn = status.endIn;
					found = true;
					break;
				}
			}
			if(!found) {
				statuses.push_back(status);
			}
			break;
		case ST_DAMAGE_OVER_TIME: case ST_HEAL_OVER_TIME:
			status.value = info;
			statuses.push_back(status);
			break;
		}	
	}
	void addAction(Action action) {
		actions.push_back(action);
	}
	vector<Action> getActions() {
		return actions;
	}
	vector<ActionDealt> useAction(Action action, vector<Character>& list, int attackIndex) {
		vector<ActionDealt> adv;
		if (action.type == AT_ULTIMATE && energy < maxEnergy) return adv;
		energy += action.energyGain;
		if (energy >= maxEnergy) {
			energy = maxEnergy;
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
			int valueHolder = value;
			value *= current.multiplier;
			vector<int> affected = vector<int>();
			switch (current.affect) {
			case AF_SINGLE_TARGET:
				affected.push_back(attackIndex);
				break;
			case AF_BLAST:
				if (attackIndex >= 1 && list.at(attackIndex - 1).isAlive) {
					affected.push_back(attackIndex - 1);
				}
				affected.push_back(attackIndex);
				if (attackIndex < list.size() - 2 && list.at(attackIndex + 1).isAlive) {
					affected.push_back(attackIndex + 1);
				}
				break;
			case AF_AOE:
				for (int j = 0; j < list.size(); j++) {
					if(list.at(j).isAlive) affected.push_back(j);
				}
				break;
			case AF_RANDOM:
				random_device rd;
				mt19937 gen(rd());
				uniform_int_distribution<int> dis(0, list.size() - 1);
				for (int j = 0; j < current.special; j++) {
					int k = dis(gen);
					if(list.at(k).isAlive) affected.push_back(k);
				}
				break;
			}
			for (int j = 0; j < affected.size(); j++) {
				Character &who = list.at(affected.at(j));
				who.addEnergy(5);
				who.checkStatuses(false);
				switch (current.purpose) {
				case AP_ATTACK:
					adv.push_back(ActionDealt(who.name, current.purpose, who.takeDamage(getOutgoingDamage(value))));
					break;
				case AP_HEAL:
					adv.push_back(ActionDealt(who.name, current.purpose, who.heal(getOutgoingDamage(value))));
					break;
				}
				int info;
				switch (current.scaling) {
				case SCT_ATK:
					info = atk;
					break;
				case SCT_DEF:
					info = def;
					break;
				case SCT_HP:
					info = hp;
					break;
				case SCT_SHIELD:
					info = getShield();
					break;
				}
				switch (current.status.type) {
				case ST_NONE:
					break;
				default:
					list.at(attackIndex).inflictStatus(current.status, info);
					break;
				}
			}
		}
		return adv;
	}
	void readStatusesCount() {
		cout << statuses.size() << endl;
	}
	int getShield() {
		int value = 0;
		for (int i = 0; i < statuses.size(); i++) {
			if (statuses.at(i).type == ST_SHIELDED) value += statuses.at(i).value;
		}
		return value;
	}
	int getOutgoingDamage(int damage) {
		double value = 1;
		for (int i = 0; i < statuses.size(); i++) {
			if (statuses.at(i).type == ST_OUTGOING_DMG_MULTIPLIER) value *= statuses.at(i).multiplier;
			if (statuses.at(i).type == ST_OUTGOING_DMG_VALUE) damage += statuses.at(i).value;
		}
		damage = damage * value;
		return damage;
	}
	int getOutgoingHeal(int heal) {
		double value = 1;
		for (int i = 0; i < statuses.size(); i++) {
			if (statuses.at(i).type == ST_OUTGOING_HEAL_MULTIPLIER) value *= statuses.at(i).multiplier;
			if (statuses.at(i).type == ST_OUTGOING_HEAL_VALUE) heal += statuses.at(i).value;
		}
		heal = heal * value;
		return heal;
	}
	void displayActions() {
		for (int i = 0; i < actions.size(); i++) {
			Action action = actions.at(i);
			switch (action.type) {
			case AT_BASIC:
				cout << "1. " << action.name << endl;
				cout << "Regeneruje punkt umiejetnosci." << endl;
				break;
			case AT_SKILL:
				cout << "2. " << action.name << endl;
				cout << "Wymaga punktu umiejetnosci." << endl;
				break;
			case AT_ULTIMATE:
				cout << "3. " << action.name << endl;
				cout << "Wymaga energii: " << energy << "/" << maxEnergy << endl;
				break;
			}
			cout << "\n===\n" << endl;
		}
	}
	void displayFullActions() {
		cout << "\n===\n" << endl;
		for (int i = 0; i < actions.size(); i++) {
			Action action = actions.at(i);
			switch (action.type) {
			case AT_BASIC:
				cout << action.name << endl << endl;
				cout << action.desc << endl << endl;
				cout << "Regeneruje punkt umiejetnosci." << endl;
				break;
			case AT_SKILL:
				cout << action.name << endl << endl;
				cout << action.desc << endl << endl;
				cout << "Wymaga punktu umiejetnosci." << endl;
				break;
			case AT_ULTIMATE:
				cout << action.name << endl << endl;
				cout << action.desc << endl << endl;
				cout << "Wymaga energii: " << energy << "/" << maxEnergy << endl;
				break;
			}
			cout << "\n===\n" << endl;
		}
	}
	void displayInfo() {
		cout << endl;
		cout << "===" << endl;
		cout << name << endl;
		cout << desc << endl;
		cout << endl;
		cout << "Statystyki na poziomie" << level << ": " << endl;
		cout << "Energia: " << maxEnergy << endl;
		cout << "Punkty Zdrowia: " << maxhp << endl;
		cout << "Atak: " << atk << endl;
		cout << "Obrona: " << def << endl;
		cout << actions.at(0).name;
		cout << actions.at(1).name;
		cout << endl;
		displayFullActions();
	}
};