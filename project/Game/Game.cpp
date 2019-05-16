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


    std::shared_ptr<Character> test_char = std::make_shared<Scout>(45);
    battle_field.update_cell(test_char, 45);
    test_char->draw_character(window, battle_field);
    window.draw(test_char->sprite);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseMoved: {
                    sf::Vector2i new_pos = sf::Mouse::getPosition(window);
                    sf::Vector2f pos_moved(new_pos.x, new_pos.y);
                    int hvd_cell_id = battle_field.get_cell_id_from_pos(pos_moved);
                    if (!battle_field.is_empty(hvd_cell_id) and !active) {
                        move_area = battle_field.find_move_area(hvd_cell_id, matrix, distance);
                        sf::Color color(20, 30, 82, 100);
                        battle_field.drop_highlight_cells();
                        battle_field.add_highlight_cells(move_area[distance], color, color);
                        //std::cout << hvd_cell_id << std::endl;
                    } else if (!active){                    
                        battle_field.drop_highlight_cells();
                    }
                    break;
                }    

                case sf::Event::MouseButtonPressed:
                    pos_pressed = sf::Mouse::getPosition(window);
                    break;

                case sf::Event::MouseButtonReleased:
                    pos_released = sf::Mouse::getPosition(window);
                    if (pos_pressed == pos_released) {
                        sf::Vector2f pos(pos_pressed.x, pos_pressed.y);
                        int cell_id = battle_field.get_cell_id_from_pos(pos);
                        if (active and !battle_field.is_in_area(move_area, cell_id)) { //если был клик не в зоне, снятие подсветки, деактивация персонажа
                            battle_field.drop_highlight_cells();
                            active = false;
                        }
                        if (battle_field.is_passable(cell_id)) {
                        //эта клетка проходима 
                            sf::Color color_trace(20, 240, 45, 100);
                            if (battle_field.is_in_area(move_area, cell_id) and active and battle_field.is_empty(cell_id)) {
                                // Эта клетка в подсвеченной зоне и есть активная клетка
                                // Значит надо рисовать путь
                                //battle_field.set_active_char(cell_id, false);
                                std::vector<int> route = battle_field.find_route(cell_id, move_area, matrix);
                                active = false;
                                battle_field.drop_highlight_cells();
                                battle_field.add_highlight_cells(route, color_trace, color_trace);
                                battle_field.update_cell(test_char, route[0]);

                            } else if (!battle_field.is_empty(cell_id)) {
                                // Нет активной клетки и нарисованной зоны
                                // Значит надо рисовать зону
                                move_area = battle_field.find_move_area(cell_id, matrix, distance);
                                active = true;
                                sf::Color color(20, 30, 52, 150);
                                battle_field.drop_highlight_cells();
                                battle_field.add_highlight_cells(move_area[distance], color, color);
                                battle_field.add_highlight_cells({move_area[0][0]}, color_trace, color_trace);
                            }
                        }
                    }
                    break;
    
                default: 
                    break;
            }
        }
        test_char->animate();
        battle_field.draw_map(window);
        test_char->draw_character(window, battle_field);
        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(160));
    }
}
