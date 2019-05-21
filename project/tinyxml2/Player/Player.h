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
    virtual bool make_turn(class Map& btl_fld, sf::Event event, sf::RenderWindow& window) = 0;
    //virtual bool move_character(std::vector<int> id, std::shared_ptr<Character> character); // обновляет состояние клетки и передвигает персонажа, здесь же строится путь, если недостижимо, то возвращает false
    //virtual void attack_with_character(std::shared_ptr<Character> my_char, std::shared_ptr<Character> target_char);
    std::shared_ptr<Character> get_char(const int index);
    int get_chars_size();
    int get_active_char_index();
    int get_char_index_from_cell(int cell_id);
    void deactivate_all_chars();
    
    ~Player() = default;
 protected:
    sf::Color color_trace = sf::Color(20, 240, 45, 100);
    sf::Color hover_color = sf::Color(32, 30, 52, 70);
    sf::Color color = sf::Color(12, 30, 52, 120);
    std::vector<std::shared_ptr<Character>> chars;
    std::vector<std::shared_ptr<Cell>> assigned_cells; // Клетки, доступные игроку
};

class Human : public Player {
 public:
    Human(class Map field);
    sf::Vector2i pos_pressed;
    sf::Vector2i pos_released;
    bool make_turn(class Map& btl_fld, sf::Event event, sf::RenderWindow& window) override;
};

class Bot : public Player {
 public:   
    bool make_turn(class Map& btl_fld, sf::Event event, sf::RenderWindow& window) override;
    Bot(class Map field);
};

#endif //ONEMORETHING_PLAYER_H