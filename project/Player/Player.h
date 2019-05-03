//
// Created by arugaf on 03.05.19.
//

#ifndef ONEMORETHING_PLAYER_H
#define ONEMORETHING_PLAYER_H

#include "Character.h"
#include "Map.h"

enum Current_player {
    PLAYER1,
    PLAYER2
};

class Player {
 public:
    virtual void move_character();

protected:
    // std::vector<std::shared_ptr<Character>> assigned_characters;
};

class User : public Player {

};

class Bot : public Player {

};

#endif //ONEMORETHING_PLAYER_H
