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
    //window.setFramerateLimit(10);


    Map btl_fld("../source/game_map/" + xml_file_path);    

    std::vector<std::vector<int>> move_area(1);                           //инициализация вектора пути
    std::vector<std::vector<int>> matrix = btl_fld.get_adj_matrix(); //пересчет матрицы смежности должен быть не здесь
    
    sf::Vector2i pos_pressed(0, 0);
    sf::Vector2i pos_released(0, 0);

    int distance = 5;           // радиус хода (надо брать из персонажа)
    bool active = false;        // выбрана ли клетка ()


    std::shared_ptr<Character> test_char = std::make_shared<Scout>(45);
    btl_fld.update_cell(test_char, 45);

    while (window.isOpen()) {
        if (!window.hasFocus()) {
            continue;
        }
        sf::Event event;
        while (window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseMoved: {
                    sf::Vector2i new_pos = sf::Mouse::getPosition(window);
                    sf::Vector2f pos_moved(new_pos.x, new_pos.y);
                    int hvd_cell_id = btl_fld.get_cell_id_from_pos(pos_moved);
                    if (!btl_fld.is_empty(hvd_cell_id) and !test_char->is_active()) {
                        move_area = btl_fld.find_move_area(hvd_cell_id, matrix, distance);
                        sf::Color color(20, 30, 82, 100);
                        btl_fld.drop_highlight_cells();
                        btl_fld.add_highlight_cells(move_area[distance], color, color);
                        //std::cout << hvd_cell_id << std::endl;
                    } else if (!test_char->is_active()) {                    
                        btl_fld.drop_highlight_cells();
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
                        int cell_id = btl_fld.get_cell_id_from_pos(pos);
                        if (test_char->is_active() and !btl_fld.is_in_area(move_area, cell_id)) { 
                        //если был клик не в зоне, снятие подсветки, деактивация персонажа
                            btl_fld.drop_highlight_cells();
                            test_char->set_active(false);
                        }
                        if (btl_fld.is_passable(cell_id)) {
                        //эта клетка проходима 
                            sf::Color color_trace(20, 240, 45, 100);
                            if (btl_fld.is_in_area(move_area, cell_id) and test_char->is_active() and btl_fld.is_empty(cell_id)) {
                                /*Эта клетка в подсвеченной зоне и есть активная клетка
                                Значит надо рисовать путь */
                                //btl_fld.set_active_char(cell_id, false);
                                std::vector<int> route = btl_fld.find_route(cell_id, move_area, matrix);
                                test_char->set_active(false);
                                btl_fld.drop_highlight_cells();
                                btl_fld.add_highlight_cells(route, color_trace, color_trace);
                                btl_fld.update_cell(test_char, route[0]);

                            } else if (!btl_fld.is_empty(cell_id)) {
                                /*Нет активной клетки и нарисованной зоны
                                Значит надо рисовать зону*/
                                move_area = btl_fld.find_move_area(cell_id, matrix, distance);
                                test_char->set_active(true);
                                sf::Color color(20, 30, 52, 150);
                                btl_fld.drop_highlight_cells();
                                btl_fld.add_highlight_cells(move_area[distance], color, color);
                                btl_fld.add_highlight_cells({move_area[0][0]}, color_trace, color_trace);
                            }
                        }
                    }
                    break;
    
                default: 
                    break;
            }
        }

        btl_fld.draw_map(window);
        test_char->draw_character(window, btl_fld);
        test_char->animate();
        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(160));
    }
}
