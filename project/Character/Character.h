//
// Created by arugaf on 02.05.19.
//

#ifndef ONEMORETHING_CHARACTER_H
#define ONEMORETHING_CHARACTER_H

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
    //Character() {};
    //virtual unsigned int apply_damage(std::shared_ptr <Cell> cell) = 0; // Вызывает move_character из Player, если получает false, то возвращает false
    //virtual void get_damage(unsigned int damage) = 0;

    //void move(int id); // Передвигает на одну! клетку

    //void set_active(); // Обновляет статус текущего персонажа и обнуляет предыдущего активного
    //void set_inactive();
    //bool is_active();

    int get_current_cell();
    void update_id(int id);
    void draw_character(sf::RenderWindow& window, class Map field);
    void animate();

    sf::Sprite sprite; // будет в протектед
 protected:
    int cell_id;

    Status status;

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

class Scout : virtual public Character {
 public:
    Scout(const int id);

    //  unsigned int apply_damage(std::shared_ptr <Cell> cell) override;
    //  void get_damage(unsigned int damage) override;
 private:
    float scale = 0.11;
    sf::Texture idle_texture;
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
