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
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "One More Thing", sf::Style::Fullscreen);
    //sf::RenderWindow window(sf::VideoMode(1920, 750), "One More Thing");
    window.setFramerateLimit(8);

    Map btl_fld("../source/game_map/" + xml_file_path);    
    sf::Color color_trace(20, 240, 45, 100);
    sf::Color color(20, 30, 52, 150);

    std::vector<std::vector<int>> move_area;
    std::vector<std::vector<int>> matrix = btl_fld.get_adj_matrix();

    players.push_back(std::make_unique<User>());
    players.push_back(std::make_unique<Bot>());

    // переход хода будет осуществляться в обработчике событий после любого действия, приводящего к передвижению/действию персонажа через переменную current_player

    // выбор активных персонажей должен будет осуществляться по значению указателя в клетке

    btl_fld.update_cell(players[current_player]->assigned_characters[0], 45);
    btl_fld.update_cell(players[current_player]->assigned_characters[1], 20);
    btl_fld.update_cell(players[1]->assigned_characters[0], 200);
    btl_fld.update_cell(players[1]->assigned_characters[1], 300);
    players[current_player]->assigned_characters[0]->draw_character(window, btl_fld);
    players[1]->assigned_characters[0]->draw_character(window, btl_fld);


    std::shared_ptr<Character> test_char = std::make_shared<Scout>(45);
    btl_fld.update_cell(test_char, 45);
    
    sf::Vector2i pos_pressed;
    sf::Vector2i pos_released;
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
                    int hvd_cell_id = btl_fld.get_cell_id_from_pos(sf::Vector2f(new_pos));
                    if (!btl_fld.is_empty(hvd_cell_id) and !test_char->is_active()) {
                        move_area = btl_fld.find_move_area(hvd_cell_id, matrix, test_char->get_mv_range());
                        sf::Color color(20, 30, 82, 100);
                        btl_fld.drop_highlight_cells();
                        btl_fld.add_highlight_cells(move_area[test_char->get_mv_range()], color, color);
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
                    if (btl_fld.compare_positions(sf::Vector2f(pos_pressed), sf::Vector2f(pos_released))) {
                        sf::Vector2f pos(pos_pressed.x, pos_pressed.y);
                        int cell_id = btl_fld.get_cell_id_from_pos(sf::Vector2f(pos_pressed));
                        if (test_char->is_active() and !btl_fld.is_in_area(move_area, cell_id)) { 
                        //если был клик не в зоне, снятие подсветки, деактивация персонажа
                            btl_fld.drop_highlight_cells();
                            test_char->set_active(false);
                        }
                        if (!btl_fld.is_empty(cell_id) and !test_char->is_active()) {
                            move_area = btl_fld.find_move_area(cell_id, matrix, test_char->get_mv_range());
                            test_char->set_active(true);
                            btl_fld.drop_highlight_cells();
                            btl_fld.add_highlight_cells(move_area[test_char->get_mv_range()], color, color);
                            btl_fld.add_highlight_cells({move_area[0][0]}, color_trace, color_trace);   
                        } else if (test_char->is_active() and btl_fld.is_in_area(move_area, cell_id)) {
                            std::vector<int> route = btl_fld.find_route(cell_id, move_area, matrix);
                            test_char->set_active(false);
                            btl_fld.drop_highlight_cells();
                            btl_fld.add_highlight_cells(route, color_trace, color_trace);
                            btl_fld.update_cell(test_char, route[0]);
                            matrix = btl_fld.get_adj_matrix();
                        }
                    }

                    break;
    
                default: 
                    break;
            }
        }

        btl_fld.draw_map(window);
        test_char->draw_character(window, btl_fld);
        players[0]->assigned_characters[0]->draw_character(window, btl_fld);
        players[1]->assigned_characters[0]->draw_character(window, btl_fld);
        players[0]->assigned_characters[0]->animate();
        players[1]->assigned_characters[0]->animate();
        test_char->animate();
        window.display();

        //std::this_thread::sleep_for(std::chrono::milliseconds(160)); // установлен лимит кадров средсвами SFML
    }
}
