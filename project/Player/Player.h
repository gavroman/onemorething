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

class Player;
class Character;
struct Cell;

// static std::vector<std::unique_ptr<Player>> players; // Скорее всего будет в мэйне, но это не точно

class Player {
 public:
    virtual void move_character(std::vector<int> id); // обновляет состояние клетки и передвигает персонажа
    std::shared_ptr<Character> get_active_char();
    bool is_my_char(std::shared_ptr<Character>);

    /*std::shared_ptr<Character> get_char_ptr(int id);
    int get_number_of_characters();*/

 protected:
    std::vector<std::shared_ptr<Character>> assigned_characters; // Персонажи игрока
    std::vector<std::shared_ptr<Cell>> assigned_cells; // Клетки, доступные игроку
};

class User : public Player {

};

class Bot : public Player {

};

#endif //ONEMORETHING_PLAYER_H
