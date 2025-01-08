#include <iostream>
#include "characters.cpp"

using namespace std;

int main()
{
    vector<Character> listOfCharacters;
    vector<Character> listOfEnemies;
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
    desc += "Dodatkowo zwiększa Atak Wojownika\no 40% na 2 tury.";
    action = Action(
        "Wojownicza Szarża",
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

}