//
// Created by arugaf on 02.05.19.
//

#ifndef ONEMORETHING_CHARACTER_H
#define ONEMORETHING_CHARACTER_H

#define SOURCE "../source/characters/"

#include "Map.h"
#include "Player.h"

enum Status {IDLE, WALK, ATTACK, HURT, DYING, DEAD, HEAL};

class Character {
 public:
    Character();
    //virtual unsigned int apply_damage(std::shared_ptr <Cell> cell) = 0; // Вызывает move_character из Player, если получает false, то возвращает false
    //virtual void get_std::unique_ptr<sf::RenderWindow> draw(std::unique_ptr<sf::RenderWindow> window);damage(unsigned int damage) = 0;
    //void move(int id); // Передвигает на одну! клетку
    virtual void do_damage(std::shared_ptr<Character> character);
    virtual void do_heal(std::shared_ptr<Character> character);
    void set_attack_target(std::shared_ptr<Character> character);

    void set_move_area(std::vector<std::vector<int>>& area);
    void set_route(std::vector<int>& way);
    void set_active(const bool active_stmt);
    bool is_active();
    bool is_idle();

    int get_current_cell();
    int get_mv_range();
    bool get_inverse();
    std::vector<std::vector<int>> get_move_area();
    int get_status();

    void update_id(const int id);
    void move(std::vector<int> way, class Map field);
    void draw(sf::RenderWindow& window, class Map field);
    void animate();
    int get_hp();
    int get_max_hp();
    int get_max_damage();
    bool get_range();
    int get_heal();

 protected:
    Status status;
    int cell_id;
    bool active = false;
    bool inverse = false;
    std::vector<std::vector<int>> move_area;
    std::vector<int> route;
    bool need_attack = false;
    std::shared_ptr<Character> attack_target = nullptr;

    int hp;
    int max_hp;
    int damage_min;
    int damage_max;
    int move_range;
    bool range = false;
    int heal = 0;

    sf::Sprite sprite;
    sf::Texture idle_texture;
    sf::Texture walk_texture;
    sf::Texture hurt_texture;
    sf::Texture attack_texture;
    sf::Texture die_texture;
    sf::Texture heal_texture;

    int texture_x;
    int texture_y;
    int texture_height;
    int texture_width;
    int sprites_amount;
    float scale;
    float idle_walk_offset_x;
    float idle_walk_offset_y;
    float hurt_offset_x;
    float hurt_offset_y;
    float attack_offset_x;
    float attack_offset_y;
    float reverse_offset;

 private:
    int animation_steps = 4;
    int current_animate_index = 0;               //for move animation
    std::vector<sf::Vector2f> animate_positions; //for move animation   
};

class Melee : public virtual Character {
 public:
    // Melee();
    //  unsigned int apply_damage(std::shared_ptr <Cell> cell) override;
    //  void get_damage(unsigned int damage) override;
};

class Range : public virtual Character {

};

class Scout : public Melee {
 public:
    Scout(const int id, const int current_player);
};

class Archer : public Range {
 public:
    Archer(const int id, const int current_player);
};

class Swordman : public Melee {
 public:
    Swordman(const int id, const int current_player);
};

class Tank : public Melee {
 public:
    Tank(const int id, const int current_player);
};

class Wizard : public Range {
 public:
    Wizard(const int id, const int current_player);
};

class Berserker : public Melee {
 public:
    Berserker(const int id, const int current_player);
};

class Knight : public Melee {
 public:
    Knight(const int id, const int current_player);
};

class Healer : public Melee, public Range {
 public:
    Healer(const int id, const int current_player);
};

#endif //ONEMORETHING_CHARACTER_H
