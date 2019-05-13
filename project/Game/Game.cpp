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
    //sf::RenderWindow window(sf::VideoMode(1920, 750), "One More Thing");

    Map battle_field("../source/game_map/" + xml_file_path);
    battle_field.draw_map(window);

    std::vector<std::vector<int>> move_area(1);                           //инициализация вектора пути
    std::vector<std::vector<int>> matrix = battle_field.get_adj_matrix(); //пересчет матрицы смежности должен быть не здесь
    
    sf::Vector2i pos_pressed(0, 0);
    sf::Vector2i pos_released(0, 0);

    int distance = 5;           // радиус хода (надо брать из персонажа)
    bool active = false;        // выбрана ли клетка ()

    battle_field.draw_map(window);
    window.display();

    std::shared_ptr<Character> test_char = std::make_shared<Scout>();
    battle_field.update_cell(test_char, 447);

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
                        if (battle_field.is_passable(cell_id)) {
                        //эта клетка проходима 
                            battle_field.draw_map(window);
                            sf::Color color_trace(20, 240, 45, 225);
                            if (battle_field.is_in_area(move_area, cell_id) and active) {
                                // Эта клетка в подсвеченной зоне и есть активная клетка
                                // Значит надо рисовать путь
                                std::vector<int> route = battle_field.find_route(cell_id, move_area, matrix);
                                for (auto& it : route) {
                                    window.draw(battle_field.highlight_cell(it, color_trace, color_trace));
                                }
                                active = false;
                            } else {
                                // Нет активной клетки и нарисованной зоны
                                // Значит надо рисовать зону
                                move_area = battle_field.find_move_area(cell_id, matrix, distance);
                                active = true;
                                sf::Color color(20, 30, 52, 150);
                                for (auto& it : move_area[distance]) {
                                    if (it == cell_id) { // подсветка активной клетки другим цветом
                                        window.draw(battle_field.highlight_cell(cell_id, color_trace, color_trace));
                                        continue;
                                    }
                                    window.draw(battle_field.highlight_cell(it, color, color));
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
