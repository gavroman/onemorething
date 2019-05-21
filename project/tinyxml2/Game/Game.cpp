//
// Created by arugaf on 08.05.19.
//
#include "Game.h"

extern std::vector<std::unique_ptr<Player>> players;

Game::Game(const int &map_id) {
    maps = {"Dark_map.tmx",
            "Grass_map.tmx",
            "Snow_map.tmx",
            "Mixed_map.tmx",
            "Mixed_map_v2.tmx"};
    run_game(maps[map_id]);
}

void Game::run_game(const std::string xml_file_path) {
    //sf::RenderWindow window(sf::VideoMode(1920, 1080), "One More Thing", sf::Style::Fullscreen);
    sf::RenderWindow window(sf::VideoMode(1920, 750), "One More Thing");
    window.setFramerateLimit(12);

    Map btl_fld("../source/game_map/" + xml_file_path);    


    players.push_back(std::make_unique<Human>(btl_fld)); //создание игроков
    players.push_back(std::make_unique<Bot>(btl_fld));
    Current_player curr_plr = PLAYER1;

    while (window.isOpen()) {
        if (!window.hasFocus()) {
            continue;
        }
        sf::Event event;
        while (window.pollEvent(event)) {
            if (players[curr_plr]->make_turn(btl_fld, event, window)) {
                curr_plr = (curr_plr == PLAYER1)? PLAYER2: PLAYER1;
                std::cout << "Current player = " << curr_plr << std::endl;
            }
        }    
        //std::cout << "pre draw" << std::endl;
        btl_fld.draw(window);
        for (int i = 0; i != players[PLAYER1]->get_chars_size(); i++) {
            players[PLAYER1]->get_char(i)->draw(window, btl_fld);
            players[PLAYER2]->get_char(i)->draw(window, btl_fld);
            players[PLAYER1]->get_char(i)->animate();
            players[PLAYER2]->get_char(i)->animate();
        }
        //std::cout << "post draw" << std::endl;
        window.display();
        //std::this_thread::sleep_for(std::chrono::milliseconds(160)); // установлен лимит кадров средсвами SFML
    }
}