//
// Created by arugaf on 02.05.19.
//

#ifndef ONEMORETHING_CHARACTER_H
#define ONEMORETHING_CHARACTER_H

#include "Map.h"
#include "Player.h"

enum Status {
    IDLE = 0,
    WALK = 1,
    ATTACK = 2,
    HURT = 3,
    DEAD = 4,
};

class Character {
 public:
    //virtual unsigned int apply_damage(std::shared_ptr <Cell> cell) = 0; // Вызывает move_character из Player, если получает false, то возвращает false
   // virtual void get_damage(unsigned int damage) = 0;

    //void move(int id); // Передвигает на одну! клетку

    //void set_active(); // Обновляет статус текущего персонажа и обнуляет предыдущего активного
    //void set_inactive();
    //bool is_active();

    //int get_current_cell();

    void animate();
    void draw(sf::RenderWindow& window); // ВРЕМЕННАЯ ФУНКЦИЯ

    sf::Sprite sprite; // будет в протектед
 protected:
    Status status;
    int current_cell;

    sf::Texture idle_texture;
    sf::Texture walk_texture;

    int texture_x;
    int texture_y;
    int texture_height;
    int texture_width;

    unsigned int hp;
    unsigned int damage_min;
    unsigned int damage_max;
    unsigned int range;

    bool active;
};

class Melee : public Character {
 public:
    Melee() = default;
};

class Range : public Character {

};

class Scout : public Character {
 public:
    Scout();
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
