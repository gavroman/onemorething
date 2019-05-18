//
// Created by arugaf on 18.05.19.
//

#include "Player.h"

Player::Player() {
    assigned_characters.push_back(std::make_shared<Scout>(0));
    assigned_characters.push_back(std::make_shared<Swordman>());
    // В нормальном конструкторе будет инициализация в зависимости от выбранных персонажей и присваиванием им нужных текстур в зависимости от пользователя
}
