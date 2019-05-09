//
// Created by arugaf on 03.05.19.
//

#ifndef ONEMORETHING_PLAYER_H
#define ONEMORETHING_PLAYER_H

#include "Character.h"
#include "Map.h"

enum Current_player {
    PLAYER1 = 0,
    PLAYER2 = 1
};

static Current_player current_player = PLAYER1;

class Character;
struct Cell;

class Player {
 public:
    Player() = default;

    virtual bool move_character(std::vector<int> id, std::shared_ptr<Character>); // обновляет состояние клетки и передвигает персонажа, здесь же строится путь, если недостижимо, то возвращает false
    std::shared_ptr<Character> get_active_char();
    bool is_my_char(std::shared_ptr<Character> character);

    /*std::shared_ptr<Character> get_char_ptr(int id);
    int get_number_of_characters();*/

    ~Player() = default;
 protected:
    std::vector<std::shared_ptr<Character>> assigned_characters; // Персонажи игрока
    std::vector<std::shared_ptr<Cell>> assigned_cells; // Клетки, доступные игроку
};

class User : public Player {

};

class Bot : public Player {

};

// std::vector <std::unique_ptr <Player>> players;

#endif //ONEMORETHING_PLAYER_H
