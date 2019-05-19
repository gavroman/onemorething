//
// Created by arugaf on 08.05.19.
//

#ifndef ONEMORETHING_GAME_H
#define ONEMORETHING_GAME_H

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

#include "Player.h"
#include "Map.h"
#include "Character.h"


class Game {
 public:
    Game() = default;
    Game(const int& map_id); // Пока не сделано меню

    void show_menu();
    void run_game(const std::string xml_file_path);
    void pause_game();

    // std::vector<std::unique_ptr<Player>> players;

    ~Game() = default;

 private:
    std::vector<std::string> maps;
};


#endif //ONEMORETHING_GAME_H
