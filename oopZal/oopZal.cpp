#include <iostream>
#include "characters.cpp"

using namespace std;

int main()
{
    Character test = Character(
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
    test.addAction(Action(
        "Magiczny Pocisk",
        "Wystrzliwywuje magiczny pocisk zadajacy obrazenia jednemu przeciwnikowi wynoszace 100% Ataku Czarodzieja.",
        AT_BASIC,
        15,
        1.0,
        ATT_SINGLE_TARGET,
        AP_ATTACK,
        SCT_ATK,
        Status(0, 0, ST_NONE, 0),
        0
    ));
    string desc = "Wystrzliwywuje kule ognia zadajaca obrazenia jednemu przeciwnikowi wynoszace 250% Ataku Czarodzieja ";
    desc += "oraz zadaje obrazenia przeciwnikom sasiadujacym wynoszace 150% Ataku Czarodzieja.\n";
    desc += "Dodatkowo naklada efekt podpalenia na tych przeciwnikach zadajacy obrazenia wynoszace 100% Ataku Czarodzieja.\n";
    desc += "Ten efekt trwa 2 tury.";
    Action action = Action(
        "Kula Ognia",
        desc,
        AT_SKILL,
        35,
        2.5,
        ATT_SINGLE_TARGET,
        AP_ATTACK,
        SCT_ATK,
        Status(0, 0, ST_NONE, 0),
        0
    );
    action.addModifier(1.5, ATT_BLAST, AP_ATTACK, SCT_ATK, Status(0, 1.0, ST_DAMAGE_OVER_TIME, 2), 0);
    test.addAction(action);
    desc = "Przyzywa mocarna fale uderzeniowa.";
    desc += "Fala ta zadaje obrazenia wynoszace 400% Ataku Czarodzieja kazdemu przeciwnikowi.";
    desc += "Dodatkowo zadaje kolejne obrazenia wynoszace 200% Ataku Czarodzieja 3 losowym przeciwnikom.";
    action = Action(
        "Fala Zniszczenia",
        desc,
        AT_ULTIMATE,
        10,
        4.0,
        ATT_SINGLE_TARGET,
        AP_ATTACK,
        SCT_ATK,
        Status(0, 0, ST_NONE, 0),
        0
    );
    action.addModifier(2.0, ATT_RANDOM, AP_ATTACK, SCT_ATK, Status(), 0);
    test.addAction(action);

}