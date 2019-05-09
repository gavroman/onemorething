//
// Created by arugaf on 08.05.19.
//

#include "Game.h"

Game::Game(const int &map_id) {
    maps = {"Dark_map.tmx",
            "Grass_map.tmx",
            "Snow_map.tmx",
            "Mixed_map.tmx",
            "Mixed_map_v2.tmx"};
    run_game(maps[map_id]);
}

void Game::run_game(const std::string xml_file_path) {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "One More Thing", sf::Style::Fullscreen);
    // sf::RenderWindow window(sf::VideoMode(1920, 750), "One More Thing");

    Map battle_field("../source/game_map/" + xml_file_path);

    sf::Vector2i pos_pressed(0, 0);
    sf::Vector2i pos_released(0, 0);

    battle_field.draw_map(window);
    window.display();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseButtonPressed:
                    pos_pressed = sf::Mouse::getPosition(window);
                    break;

                case sf::Event::MouseButtonReleased:
                    pos_released = sf::Mouse::getPosition(window);
                    if (pos_pressed == pos_released and pos_pressed.x and pos_pressed.y) {
                        sf::Vector2f pos(pos_pressed.x, pos_pressed.y);
                        int cell_id = battle_field.get_cell_id_from_pos(pos);
                        if (cell_id >= 0) {
                            window.clear();

                            battle_field.draw_map(window);
                            sf::Color color(20, 30, 52, 100);
                            window.draw(battle_field.highlight_cell(cell_id, color, sf::Color::Cyan));

                            window.display();
                            std::cout << "Pressed cell id = " << cell_id <<std::endl;
                        }
                    }
                    break;

                default:
                    break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
