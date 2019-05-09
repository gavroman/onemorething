//
// Created by arugaf on 02.05.19.
//

#ifndef ONEMORETHING_CHARACTER_H
#define ONEMORETHING_CHARACTER_H

#include "Map.h"
#include "Player.h"

class Character {
 public:
    virtual unsigned int apply_damage(std::shared_ptr <Cell> cell) = 0; // Вызывает move_character из Player, если получает false, то возвращает false
    virtual void get_damage(unsigned int damage) = 0;

    void move(int id); // Передвигает на одну! клетку

    void set_active(); // Обновляет статус текущего персонажа и обнуляет предыдущего активного
    void set_inactive();
    bool is_active();

    int get_current_cell();

 protected:
    int current_cell;

    unsigned int hp;
    unsigned int damage_min;
    unsigned int damage_max;
    unsigned int range;

    sf::Sprite sprite;

    bool active;
};

class Melee : public Character {

};

class Range : public Character {

};

class Scout : public Melee {

};

class Archer : public Range {

};

class Swordman : public Melee {

};

class Tank : public Melee {

};

class Crosswbowman : public Range {

};

class Berserker : public Melee {

};

class Knight : public Melee {

};

class Healer : public Melee, Range {

};

#endif //ONEMORETHING_CHARACTER_H
