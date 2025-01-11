#include "characters.cpp"
#include <thread>

class GameController {
public:
    vector<Character> listOfCharacters;
    vector<Character> listOfEnemies;
    vector<Character> playerCharacters;
    vector<Character> enemyCharacters;
    int wave = 0;
    int turnCount = 0;
    int aliveAllies = 0;
    int aliveEnemies = 0;
    int accumulatedXP = 0;
    bool playerTurn = true;
    short maxCharactersInTeam = 4;
    short maxSkillPoints = 5;
    short skillPointCount = 3;
    void actAsNPC(int index) {
        Character& NPC = enemyCharacters.at(index);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> disAttack(0, playerCharacters.size() - 1);
        uniform_int_distribution<int> disActions(0, NPC.getActions().size() - 1);
        int attack = disAttack(gen);
        int action = disActions(gen);
        vector<ActionDealt> adv = NPC.useAction(NPC.getActions().at(action), playerCharacters, attack);
        displayActionAgainst(NPC.name, playerCharacters.at(attack).name, NPC.getActions().at(action).name, adv);
    }
    void displayActionAgainst(string caster, string against, string actionName, vector<ActionDealt> adv) {
        cout << caster << " uzywa " << actionName << " na " << against << endl;
        for (int i = 0; i <= adv.size() - 1; i++) {
            switch (adv.at(i).purpose) {
            case AP_STATUS:
                break;
            case AP_ATTACK:
                cout << adv.at(i).name << " otrzymal " << adv.at(i).value << " punktow obrazen!" << endl;
                break;
            case AP_HEAL:
                cout << adv.at(i).name << " zostal uleczony o " << adv.at(i).value << " punktow zdrowia!" << endl;
                break;
            }
        }
    }
    void actAsPC(int index) {
        Character& PC = playerCharacters.at(index);
        cout << "Akcje podejmuje " << PC.name << endl;
        cout << "\n===\n" << endl;
        PC.displayActions();
        int action = 0;
        int attack = 0;
        while (action == 0) {
            cout << "Wybierz akcje (1-3): ";
            cin >> action;
            if (action <= 0 || action > 3) {
                action = 0;
            }
        }
        vector<ActionDealt> adv;
        Action selectedAction = PC.getActions().at(action - 1);
        while (attack == 0) {
            if (selectedAction.components.at(0).target == ATG_ALLY) {
                cout << "Wybierz na ktorym sojuszniku uzyc umiejetnosci (1-" << playerCharacters.size() << "): ";
                cin >> attack;
                if (attack <= 0 || attack > playerCharacters.size()) {
                    attack = 0;
                }
                adv = PC.useAction(PC.getActions().at(action), playerCharacters, attack);
                displayActionAgainst(PC.name, playerCharacters.at(attack).name, PC.getActions().at(attack).name, adv);
            }
            else if (selectedAction.components.at(0).target == ATG_ENEMY) {
                cout << "Wybierz na ktorym przeciwniku uzyc umiejetnosci (1-" << enemyCharacters.size() << "): ";
                cin >> attack;
                if (attack <= 0 || attack > playerCharacters.size()) {
                    attack = 0;
                }
                adv = PC.useAction(PC.getActions().at(action), enemyCharacters, attack);
                displayActionAgainst(PC.name, enemyCharacters.at(attack).name, PC.getActions().at(attack).name, adv);
            }
            else if (selectedAction.components.at(0).target == ATG_SELF) {
                attack = index;
                adv = PC.useAction(PC.getActions().at(action), playerCharacters, attack);
                displayActionAgainst(PC.name, "sobie", PC.getActions().at(attack).name, adv);
            }
        }
        
    }
    void enemysTurn() {
        cout << "Tura przeciwnika..." << endl;
        for (int i = 0; i < enemyCharacters.size() - 1; i++) {
            if (enemyCharacters.at(i).isAlive) {
                actAsNPC(i);
            }
        }
    }
    void playersTurn() {
        cout << "Twoja tura!" << endl;
        for (int i = 0; i < playerCharacters.size() - 1; i++) {
            if (playerCharacters.at(i).isAlive) {
                actAsPC(i);
            }
        }
    }
    void displayCharacterScreen() {
        for (int i = 0; i < enemyCharacters.size() - 1; i++) {
            Character enemy = enemyCharacters.at(i);
            cout << i + 1 << ": " << enemy.name << endl;
            if (enemyCharacters.at(i).isAlive) {
                cout << "HP: " << enemy.hp << endl;
            }
            else {
                cout << "HP: 0 [KO]" << endl;
            }
            cout << "\n";
        }
        cout << "\n\n===\n\n";
        for (int i = 0; i < playerCharacters.size() - 1; i++) {
            Character player = playerCharacters.at(i);
            cout << i + 1 << ": " << player.name << endl;
            if (enemyCharacters.at(i).isAlive) {
                cout << "HP: " << player.hp << " / " << player.maxhp << endl;
                cout << "Energia: " << player.energy << " / " << player.maxEnergy;
            }
            else {
                cout << "HP: 0 [KO]" << endl;
                cout << "\n";
            }
        }
        this_thread::sleep_for(2s);
    }
    void checkForEndOfBattle() {

    }
    GameController() {
        Character currentCharacter;
        Action action;
        string desc;

        // Playable
        // Czarodziej
        {
            Character currentCharacter = Character(
                "Czarodziej",
                "Mistrz zaklec i czarow.",
                PLAYABLE,
                80,
                20,
                6,
                120,
                1,
                0,
                1.2,
                1.15,
                1.1
            );
            string desc = "Wystrzliwywuje magiczny pocisk zadajacy obrazenia\n jednemu przeciwnikowi wynoszace 100% Ataku Czarodzieja.";
            currentCharacter.addAction(Action(
                "Magiczny Pocisk",
                desc,
                AT_BASIC,
                15,
                1.0,
                ATG_ENEMY,
                AF_SINGLE_TARGET,
                AP_ATTACK,
                SCT_ATK,
                Status(),
                0
            ));
            desc = "Wystrzliwywuje kule ognia zadajaca obrazenia jednemu\n przeciwnikowi wynoszace 250% Ataku Czarodzieja\n";
            desc += "oraz zadaje obrazenia przeciwnikom sasiadujacym\n wynoszace 150% Ataku Czarodzieja.\n";
            desc += "Dodatkowo naklada efekt podpalenia na tych przeciwnikach\n zadajacy obrazenia wynoszace 100% Ataku Czarodzieja.\n";
            desc += "Ten efekt trwa 2 tury.";
            Action action = Action(
                "Kula Ognia",
                desc,
                AT_SKILL,
                35,
                2.5,
                ATG_ENEMY,
                AF_SINGLE_TARGET,
                AP_ATTACK,
                SCT_ATK,
                Status(),
                0
            );
            action.addComponent(1.5, ATG_ENEMY, AF_BLAST, AP_ATTACK, SCT_ATK, Status(0, 1.0, ST_DAMAGE_OVER_TIME, 2), 0);
            currentCharacter.addAction(action);
            desc = "Przyzywa mocarna fale uderzeniowa.\n";
            desc += "Fala ta zadaje obrazenia wynoszace\n400% Ataku Czarodzieja kazdemu przeciwnikowi.\n";
            desc += "Dodatkowo zadaje kolejne obrazenia wynoszace\n200% Ataku Czarodzieja 3 losowym przeciwnikom.";
            action = Action(
                "Fala Zniszczenia",
                desc,
                AT_ULTIMATE,
                10,
                4.0,
                ATG_ENEMY,
                AF_SINGLE_TARGET,
                AP_ATTACK,
                SCT_ATK,
                Status(),
                0
            );
            action.addComponent(2.0, ATG_ENEMY, AF_RANDOM, AP_ATTACK, SCT_ATK, Status(), 0);
            listOfCharacters.push_back(currentCharacter);
        }
        // Wojownik
        {
        currentCharacter = Character(
            "Wojownik",
            ".",
            PLAYABLE,
            120,
            20,
            8,
            100,
            1,
            0,
            1.1,
            1.25,
            1.15
        );

        desc = "Atakuje jednego przeciwnika mieczem zadajac\nobrazenia wynoszace 150% Ataku Wojownika.";
        action = Action(
            "Uderzenie Mieczem",
            desc,
            AT_BASIC,
            20,
            1.5,
            ATG_ENEMY,
            AF_SINGLE_TARGET,
            AP_ATTACK,
            SCT_ATK,
            Status(),
            0
        );

        currentCharacter.addAction(action);

        desc = "Atakuje jednego przeciwnika mieczem, zadajac\nobrazenia wynoszace 350% Ataku Wojownika.\n";
        desc += "Dodatkowo oslabia przeciwnika, obnizajac\njego Obrone o 25%, ten efekt trwa 2 tury.";
        action = Action(
            "Przebijajacy Atak",
            desc,
            AT_SKILL,
            40,
            3.5,
            ATG_ENEMY,
            AF_SINGLE_TARGET,
            AP_ATTACK,
            SCT_ATK,
            Status(0, 0.75, ST_DEF_MULTIPLIER, 2),
            0
        );

        currentCharacter.addAction(action);

        desc = "Atakuje jednego przeciwnika, zadajac\nobrazenia wynoszace 450% Ataku Wojownika.\n";
        desc += "Dodatkowo zwiêksza Atak Wojownika\no 40% na 2 tury.";
        action = Action(
            "Wojownicza Szar¿a",
            desc,
            AT_ULTIMATE,
            10,
            4.5,
            ATG_ENEMY,
            AF_SINGLE_TARGET,
            AP_ATTACK,
            SCT_ATK,
            Status(),
            0
        );
        action.addComponent(
            1.4,
            ATG_SELF,
            AF_SINGLE_TARGET,
            AP_STATUS,
            SCT_ATK,
            Status(0, 1.4, ST_ATK_MULTIPLIER, 2),
            0
        );

        currentCharacter.addAction(action);

        listOfCharacters.push_back(currentCharacter);
    }
        // Kleryk
        {
            currentCharacter = Character(
                "Kleryk",
                ".",
                PLAYABLE,
                80,
                12,
                9,
                120,
                1,
                0,
                1.25,
                1.15,
                1.15
            );

            desc = "Atakuje jednego przeciwnika kosturem zadajac\nobrazenia wynoszace 15% Punktow Zdrowia Kleryka.";
            action = Action(
                "Uderzenie Kosturem",
                desc,
                AT_BASIC,
                20,
                0.15,
                ATG_ENEMY,
                AF_SINGLE_TARGET,
                AP_ATTACK,
                SCT_HP,
                Status(),
                0
            );

            currentCharacter.addAction(action);

            desc = "Leczy jednego sojusznika, \nz sila rowna 60% Punktow Zdrowia Kleryka.\n";
            action = Action(
                "Uzdrowienie",
                desc,
                AT_SKILL,
                50,
                0.6,
                ATG_ALLY,
                AF_SINGLE_TARGET,
                AP_HEAL,
                SCT_HP,
                Status(),
                0
            );

            currentCharacter.addAction(action);

            desc = "Leczy wszystkich sojusznikow, \nz sila rowna 60% Punktow Zdrowia Kleryka.\n";
            desc += "Dodatkowo naklada efekt opoznionego leczenia, \no sile rownej 40% Punktow Zdrowa Kleryka na 1 ture.";
            action = Action(
                "Masowe Uleczenie",
                desc,
                AT_ULTIMATE,
                10,
                0.6,
                ATG_ALLY,
                AF_AOE,
                AP_HEAL,
                SCT_HP,
                Status(0, 0.3, ST_HEAL_OVER_TIME, 1),
                0
            );

            currentCharacter.addAction(action);

            listOfCharacters.push_back(currentCharacter);
        }
        // Muzykant
        {
            currentCharacter = Character(
                "Muzykant",
                ".",
                PLAYABLE,
                75,
                14,
                7,
                130,
                1,
                0,
                1.2,
                1.2,
                1.15
            );

            desc = "Atakuje jednego i sasiednich przeciwnikow nuta muzyczna zadajac\nobrazenia wynoszace 50% Ataku Muzykanta.";
            action = Action(
                "Muzyczny Wstrzas",
                desc,
                AT_BASIC,
                20,
                0.5,
                ATG_ENEMY,
                AF_BLAST,
                AP_ATTACK,
                SCT_ATK,
                Status(),
                0
            );

            currentCharacter.addAction(action);

            desc = "Atakuje wszystkich przeciwnikow melodia zadajac\nobrazenia wynoszace 50% Ataku Muzykanta.\n";
            desc += "Dodatkowo obni¿a Atak o 30% na 2 tury.";
            action = Action(
                "Drazniaca Melodia",
                desc,
                AT_SKILL,
                35,
                0.6,
                ATG_ENEMY,
                AF_AOE,
                AP_ATTACK,
                SCT_ATK,
                Status(0, 0.7, ST_ATK_MULTIPLIER, 2),
                0
            );

            currentCharacter.addAction(action);

            desc = "Wzmacnia wszystkich sojusznikow,\nzwiekszajac zadawane obrazenia o 40%,\n";
            desc += "oraz zmniejsza przyjmowane obrazenia o 20%.\n";
            desc += "Efekt ten trwa 3 tury.\n";
            action = Action(
                "Muzykancka Inspiracja",
                desc,
                AT_ULTIMATE,
                15,
                0.6,
                ATG_ALLY,
                AF_AOE,
                AP_STATUS,
                SCT_ATK,
                Status(0, 1.4, ST_OUTGOING_DMG_MULTIPLIER, 3),
                0
            );

            action.addComponent(
                0.8,
                ATG_ALLY,
                AF_AOE,
                AP_STATUS,
                SCT_ATK,
                Status(0, 0.8, ST_INCOMING_DMG_MULTIPLIER, 3),
                0
            );

            currentCharacter.addAction(action);

            listOfCharacters.push_back(currentCharacter);
        }

        // Non-Playable
        // Zloczynca 1
        {
        currentCharacter = Character(
            "Zloczynca 1",
            "Zly pan...",
            NON_PLAYABLE,
            180,
            20,
            6,
            0,
            1,
            0,
            1.4,
            1.1,
            1.1
        );

        desc = "Atakuje jednego przeciwnika zadajac\nobrazenia wynoszace 100% Ataku.";
        action = Action(
            "Uderzenie Mieczem",
            desc,
            AT_BASIC,
            0,
            1.0,
            ATG_ALLY,
            AF_SINGLE_TARGET,
            AP_ATTACK,
            SCT_ATK,
            Status(),
            0
        );

        currentCharacter.addAction(action);

        action = Action(
            "Uderzenie Mieczem",
            desc,
            AT_BASIC,
            0,
            0.75,
            ATG_ALLY,
            AF_BLAST,
            AP_ATTACK,
            SCT_ATK,
            Status(),
            0
        );
    }
        
    }
};