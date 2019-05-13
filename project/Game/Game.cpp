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
    //sf::RenderWindow window(sf::VideoMode(1920, 1080), "One More Thing", sf::Style::Fullscreen);
    sf::RenderWindow window(sf::VideoMode(1920, 750), "One More Thing");

    Map battle_field("../source/game_map/" + xml_file_path);

    sf::Vector2i pos_pressed(0, 0);
    sf::Vector2i pos_released(0, 0);
    battle_field.draw_map(window);
    std::vector<std::vector<int>> trace(1);
    std::vector<std::vector<int>> matrix = battle_field.get_adj_matrix(); //пересчет матрицы смежности должен быть не здесь
    std::vector<int> check;
    for (int i = 0; i < matrix.size(); i++) {
        check.push_back(matrix[i][0]);
    }
    int distance = 5; //радиус хода
    bool k = false;
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
                    if (pos_pressed == pos_released) {
                        sf::Vector2f pos(pos_pressed.x, pos_pressed.y);
                        int cell_id = battle_field.get_cell_id_from_pos(pos);
                        if (std::find(check.begin(), check.end(), cell_id) != check.end()) {
                            battle_field.draw_map(window);
                            sf::Color color_trace(20, 240, 45, 225);
                            if (std::find(trace[trace.size() - 1].begin(), trace[trace.size() - 1].end(),
                                    cell_id) != trace[trace.size() - 1].end() and k) {
                                std::vector<int> one_trace = battle_field.get_one_trace(cell_id, trace, matrix);
                                for (int i = 0; i < one_trace.size(); i++) {
                                    window.draw(battle_field.highlight_cell(one_trace[i], color_trace, color_trace));
                                }
                                k = false;
                            } else {
                                trace = battle_field.get_trace(cell_id, matrix, distance);
                                k = true;
                                sf::Color color(20, 30, 52, 150);
                                for (int i = 0; i < trace[distance].size(); i++) {
                                    if (trace[distance][i] == cell_id) {
                                        window.draw(battle_field.highlight_cell(cell_id, color_trace, color_trace));

                                        continue;
                                    }
                                    window.draw(battle_field.highlight_cell(trace[distance][i], color, color));
                                }
                            }
                        }    
                    }
                    break;

                default: 
                    break;
            }
        }
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
