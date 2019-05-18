//
// Created by arugaf on 03.05.19.
//

#ifndef ONEMORETHING_PLAYER_H
#define ONEMORETHING_PLAYER_H

#include "Map.h"
#include "Character.h"

enum Current_player {
    PLAYER1 = 0,
    PLAYER2 = 1
};

static Current_player current_player = PLAYER1;

class Character;
struct Cell;

class Player {
 public:
    // Player(std::vector<int> characters, class Map field); // нормальный конструктор
    Player();

    //virtual bool move_character(std::vector<int> id, std::shared_ptr<Character> character); // обновляет состояние клетки и передвигает персонажа, здесь же строится путь, если недостижимо, то возвращает false
    //virtual void attack_with_character(std::shared_ptr<Character> my_char, std::shared_ptr<Character> target_char);
    std::shared_ptr<Character> get_active_char();
    bool is_my_char(std::shared_ptr<Character> character);

    /*std::shared_ptr<Character> get_char_ptr(int id);
    int get_number_of_characters();*/

    std::vector<std::shared_ptr<Character>> assigned_characters; // Персонажи игрока БУДЕТ В ПРОТЕКТЕД, НЕ В ПАБЛИК


    ~Player() = default;
 protected:
    std::vector<std::shared_ptr<Cell>> assigned_cells; // Клетки, доступные игроку
};

class User : public Player {

};

class Bot : public Player {

};

#endif //ONEMORETHING_PLAYER_H
