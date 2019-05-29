//
// Created by arugaf on 03.05.19.
//

#ifndef ONEMORETHING_PLAYER_H
#define ONEMORETHING_PLAYER_H

#include "Map.h"
#include "Character.h"

enum Current_player { PLAYER1, PLAYER2};
class Character;
struct Cell;

class Player {
 public:
    virtual bool make_turn(class Map& btl_fld, sf::RenderWindow& window) = 0;
    //virtual bool move_character(std::vector<int> id, std::shared_ptr<Character> character); // обновляет состояние клетки и передвигает персонажа, здесь же строится путь, если недостижимо, то возвращает false
    //virtual void attack_with_character(std::shared_ptr<Character> my_char, std::shared_ptr<Character> target_char);
    std::shared_ptr<Character> get_char(const int index);
    void check_hp(class Map& btl_fld);
    int get_chars_size();
    int get_active_char_index();
    int get_char_index_from_cell(int cell_id);
    void deactivate_all_chars();
    bool is_all_idle();
    std::shared_ptr<Character> get_hiller();
    bool status_heal = false;
    int find_nearest_cell(std::vector<std::vector<int>>  move_area, int enemy_cell, class Map& btl_fld);
    std::vector<int> get_my_chars(std::shared_ptr<Character> healer);
    std::vector<int> get_enemy_chars(class Map& btl_fld);
    std::vector<int> can_attack_chars(std::vector<int> enemy_chars, std::vector<int> cells, class Map& btl_fld);
    ~Player() = default;
 protected:std::unique_ptr<sf::RenderWindow> draw(std::unique_ptr<sf::RenderWindow> window);
    sf::Color color_trace = sf::Color(20, 240, 45, 100);
    sf::Color hover_color = sf::Color(32, 30, 52, 70);
    sf::Color color = sf::Color(12, 30, 52, 120);
    sf::Color color_enemy = sf::Color::Red;
    sf::Color color_my = sf::Color::Green;
    std::vector<std::shared_ptr<Character>> chars;
    std::vector<std::shared_ptr<Cell>> assigned_cells; // Клетки, доступные игроку
};

class Human : public Player {
 public:
    Human(class Map field, Current_player player, std::vector<int> characters);
    sf::Vector2i pos_pressed;
    sf::Vector2i pos_released;
    bool make_turn(class Map& btl_fld, sf::RenderWindow& window) override;
};

class Bot : public Player {
 public:   
    Bot(class Map field, Current_player player, std::vector<int> characters);
    bool make_turn(class Map& btl_fld, sf::RenderWindow& window) override;
};

#endif //ONEMORETHING_PLAYER_H
