//
// Created by arugaf on 02.05.19.
//

#ifndef ONEMORETHING_CHARACTER_H
#define ONEMORETHING_CHARACTER_H

#include "Map.h"
#include "Player.h"

class Character {
 public:
    virtual unsigned int apply_damage() = 0;
    virtual void get_damage(unsigned int damage) = 0;

 protected:
    unsigned int hp;
    unsigned int damage_min;
    unsigned int damage_max;
    unsigned int range;

    sf::Sprite sprite;
};

class Scout : public Character {

};

class Archer : public Character {

};

class Swordman : public Character {

};

class Healer : public Character {

};

class Tank : public Character {

};

class Crosswbowman : public Character {

};

class Berserker : public Character {

};

class Knight : public Character {

};

#endif //ONEMORETHING_CHARACTER_H
