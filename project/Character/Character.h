//
// Created by arugaf on 02.05.19.
//

#ifndef ONEMORETHING_CHARACTER_H
#define ONEMORETHING_CHARACTER_H

#include "Player.h"

enum Status { IDLE, WALK, ATTACK, HURT, DEAD};

class Character {
 public:
    //Character() {};
    //virtual unsigned int apply_damage(std::shared_ptr <Cell> cell) = 0; // Вызывает move_character из Player, если получает false, то возвращает false
    //virtual void get_damage(unsigned int damage) = 0;

    //void move(int id); // Передвигает на одну! клетку

    void set_active(const bool active_stmt);
    bool is_active();

    int get_current_cell();
    int get_mv_range();
    
    void update_id(const int id);
    void draw_character(sf::RenderWindow& window, class Map field);
    void animate();

 protected:
    int cell_id;
    Status status;
    bool active;

    sf::Sprite sprite;
    sf::Texture idle_texture;
    sf::Texture walk_texture;
    int texture_x;
    int texture_y;
    int texture_height;
    int texture_width;

    int hp;
    int damage_min;
    int damage_max;
    int move_range;
};

class Melee : public Character {
 public:
    Melee() = default;
};

class Range : public Character {

};

class Scout : virtual public Character {
 public:
    Scout(const int cell_id);

    //  unsigned int apply_damage(std::shared_ptr <Cell> cell) override;
    //  void get_damage(unsigned int damage) override;
 private:
    float scale = 0.11;
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
