#include "Player.h"

std::shared_ptr<Character> Player::get_char(const int index) {
	return chars[index];
}

int Player::get_char_index_from_cell(int cell_id) {
    for (int i = 0; i != chars.size(); i++) {
        if (chars[i]->get_current_cell() == cell_id) {
            return i;
        }
    }
    return -1;
}

int Player::get_active_char_index() {
    for (int i = 0; i != chars.size(); i++) {
        if (chars[i]->is_active()) {
            return i;
        }
    }
    return -1;
}

int Player::get_chars_size() {
    return chars.size();
}

void Player::deactivate_all_chars() {
    for (auto& chr : chars) {
        chr->set_active(false);
    }
}

bool Player::is_all_idle() {
    for (auto& chr : chars) {
        if (chr->get_status() != IDLE) {
            return false;
        }    
    }
    return true;
}

std::vector<int> Player::get_enemy_chars(class Map& field) {
    std::vector<int> my_chars;
    for (int i = 0; i < get_chars_size(); i++) {
        my_chars.push_back(chars[i]->get_current_cell());
    }
    std::vector<int> all_chars = field.get_chars();
    std::vector<int> enemy_chars;

    for (int i = 0; i < all_chars.size(); i++) {
        if (std::find(my_chars.begin(), my_chars.end(), all_chars[i]) == my_chars.end()) {
            enemy_chars.push_back(all_chars[i]);
        }
    }
    return enemy_chars;
}

std::vector<int> Player::can_attack_chars(std::vector<int> enemy_chars, std::vector<int> cells, class Map& field) {
    std::vector<int> can_attack_enemy;
    for (int i = 0; i < enemy_chars.size(); i++) {
        std::vector<int> enemy_neighbors = field.search_neighbors(enemy_chars[i]);
        for (int j = 0; j < enemy_neighbors.size(); j++) {
            if (std::find(cells.begin(), cells.end(), enemy_neighbors[j]) != cells.end()) {
                can_attack_enemy.push_back(enemy_chars[i]);
            }
        }
    }
    return can_attack_enemy;
}

void Player::check_hp(class Map& field) {
    for (int i = 0; i < chars.size(); i++) {
        if (chars[i]->get_hp() <= 0) {
            field.update_cell(nullptr, chars[i]->get_current_cell());
            chars.erase(chars.begin() + i);
          //  field.update_cell(nullptr, chars[i]->get_current_cell());
        }
    }
}

Human::Human(class Map field) {
    chars.push_back(std::make_shared<Scout>(416, PLAYER1));
    std::cout << "1 character's textures loaded" << std::endl;
    chars.push_back(std::make_shared<Archer>(384, PLAYER1));
    std::cout << "2 character's textures loaded" << std::endl;
    chars.push_back(std::make_shared<Healer>(352, PLAYER1));
    std::cout << "3 character's textures loaded" << std::endl;
    chars.push_back(std::make_shared<Knight>(320, PLAYER1));
    std::cout << "4 character's textures loaded" << std::endl;
    chars.push_back(std::make_shared<Swordman>(288, PLAYER1));    
    std::cout << "5 character's textures loaded" << std::endl;
    chars.push_back(std::make_shared<Tank>(256, PLAYER1));
    std::cout << "6 character's textures loaded" << std::endl;
    chars.push_back(std::make_shared<Wizard>(224, PLAYER1));
    std::cout << "7 character's textures loaded" << std::endl;
    chars.push_back(std::make_shared<Berserker>(192, PLAYER1));
    std::cout << "8 character's textures loaded" << std::endl;
    std::cout << "PLAYER1 done" << std::endl;
    for (auto &chr : chars) {
    	field.update_cell(chr, chr->get_current_cell());
    }
    deactivate_all_chars();
}

bool Human::make_turn(class Map& btl_fld, sf::RenderWindow& window) {
    check_hp(btl_fld);
    sf::Event event;
    while (window.pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed: {
                window.close();
                break;
            }

            case sf::Event::MouseMoved: {
                sf::Vector2i new_pos = sf::Mouse::getPosition(window);
                int hvd_cell_id = btl_fld.get_cell_id_from_pos(sf::Vector2f(new_pos));
                int char_index = get_char_index_from_cell(hvd_cell_id);
                if (char_index != -1 and get_active_char_index() == -1) {
                    std::vector<std::vector<int>> move_area = btl_fld.find_move_area(hvd_cell_id, chars[char_index]->get_mv_range());
                    btl_fld.drop_highlight_cells();
                    btl_fld.add_highlight_cells(move_area[chars[char_index]->get_mv_range()], hover_color, hover_color);
                } else if (get_active_char_index() == -1) {
                    btl_fld.drop_highlight_cells();
                }    
                break;
            }

            case sf::Event::MouseButtonPressed: {
                pos_pressed = sf::Mouse::getPosition(window);
                break;
            }

            case sf::Event::MouseButtonReleased: {
                pos_released = sf::Mouse::getPosition(window);
                if (btl_fld.compare_positions(sf::Vector2f(pos_pressed), sf::Vector2f(pos_released))) {
                    sf::Vector2f pos(pos_pressed.x, pos_pressed.y);
                    int cell_id = btl_fld.get_cell_id_from_pos(sf::Vector2f(pos_pressed));
                    int char_index = get_char_index_from_cell(cell_id);
                    if (char_index != -1) {
                        //отрисовка зоны
                        std::vector<std::vector<int>> move_area = btl_fld.find_move_area(cell_id, chars[char_index]->get_mv_range());
                        chars[char_index]->set_move_area(move_area);
                        std::vector<int> attack_enemy = can_attack_chars(get_enemy_chars(btl_fld),
                                move_area[chars[char_index]->get_mv_range()], btl_fld);
                        deactivate_all_chars();
                        chars[char_index]->set_active(true);
                        btl_fld.drop_highlight_cells();
                        btl_fld.add_highlight_cells(move_area[chars[char_index]->get_mv_range()], color, color);
                        if (attack_enemy.size() != 0) {
                            btl_fld.add_highlight_cells(attack_enemy, color_enemy, color_enemy);
                        }
                        btl_fld.add_highlight_cells({move_area[0][0]}, color_trace, color_trace);   
                    } else {
                        int active_char_index = get_active_char_index();

                        if (active_char_index != -1) {
                            std::vector<std::vector<int>> move_area = chars[active_char_index]->get_move_area();
                            std::vector<int> attack_enemy = can_attack_chars(get_enemy_chars(btl_fld),
                                                                             move_area[chars[active_char_index]->get_mv_range()], btl_fld);
                            std::vector<int> neighbors_attack;
                            if (btl_fld.is_in_area({attack_enemy}, cell_id)) {
                                std::vector<int> neighbors_attack = btl_fld.area_in_area(move_area, btl_fld.search_neighbors(cell_id));
                                if (btl_fld.is_in_area({neighbors_attack}, chars[active_char_index]->get_current_cell())) {
                                    chars[active_char_index]->do_damage(btl_fld.get_character_from_id(cell_id));
                                    return true;
                                }
                                std::vector<int> route = btl_fld.find_route(neighbors_attack[0], move_area);
                                btl_fld.drop_highlight_cells();
                                btl_fld.add_highlight_cells(route, color_trace, color_trace);
                                chars[active_char_index]->move(route, btl_fld);
                                btl_fld.update_cell(chars[active_char_index], route[0]);
                                chars[active_char_index]->do_damage(btl_fld.get_character_from_id(cell_id));
                                chars[active_char_index]->set_active(false);
                                return true; //окончание хода
                            }

                            if (btl_fld.is_in_area(move_area, cell_id)) {
                                std::vector<int> route = btl_fld.find_route(cell_id, move_area);
                                btl_fld.drop_highlight_cells();
                                btl_fld.add_highlight_cells(route, color_trace, color_trace);
                                chars[active_char_index]->move(route, btl_fld);
                                btl_fld.update_cell(chars[active_char_index], route[0]);
                                chars[active_char_index]->set_active(false);
                                //std::vector<int> neigbors = btl_fld.seearch_neighbors(route[0]);


                                return true; //окончание хода
                            } else {
                                btl_fld.drop_highlight_cells();
                                chars[active_char_index]->set_active(false);
                            }    
                        }
                    } 
                }
                break;
            }

            default: 
                return false;   
            }
        }
    return false;
}

Bot::Bot(class Map field) {
    chars.push_back(std::make_shared<Scout>(31, PLAYER2));
    std::cout << "1 character's textures loaded" << std::endl;
    chars.push_back(std::make_shared<Archer>(63, PLAYER2));
    std::cout << "2 character's textures loaded" << std::endl;
    chars.push_back(std::make_shared<Healer>(95, PLAYER2));
    std::cout << "3 character's textures loaded" << std::endl;
    chars.push_back(std::make_shared<Knight>(127, PLAYER2));
    std::cout << "4 character's textures loaded" << std::endl;
    chars.push_back(std::make_shared<Swordman>(159, PLAYER2));
    std::cout << "5 character's textures loaded" << std::endl;
    chars.push_back(std::make_shared<Tank>(191, PLAYER2));
    std::cout << "6 character's textures loaded" << std::endl;
    chars.push_back(std::make_shared<Wizard>(223, PLAYER2));    
    std::cout << "7 character's textures loaded" << std::endl;
    chars.push_back(std::make_shared<Berserker>(255, PLAYER2));
    std::cout << "8 character's textures loaded" << std::endl;
    std::cout << "PLAYER2 done" << std::endl;
    for (auto &chr : chars) {
    	field.update_cell(chr, chr->get_current_cell());
    }
}

bool Bot::make_turn(class Map& btl_fld, sf::RenderWindow& window) {
    //TODO (9rik): Ярик ебашь
    check_hp(btl_fld);
    btl_fld.drop_highlight_cells();
    sf::Event event;
    while (window.pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed: {
                window.close();
                break;
            }
        }    
    }

    int max_damage = -1;
    std::shared_ptr<Character> max_character = nullptr;
    int enemy_cell = -1;

    for (int i = 0; i < chars.size(); i++) { // ищем перса который может ударить с max_атакой
        int cell_char = chars[i]->get_current_cell();
        std::vector<std::vector<int>> move_area = btl_fld.find_move_area(cell_char, chars[i]->get_mv_range());
        std::vector<int> attack_enemy = can_attack_chars(get_enemy_chars(btl_fld),
                                                         move_area[chars[i]->get_mv_range()], btl_fld);
        if (attack_enemy.size() and chars[i]->get_max_damage() > max_damage) {
            max_damage = chars[i]->get_max_damage();
            max_character = chars[i];
            enemy_cell = attack_enemy[0];
        }

    }

    if (max_character) {
        std::vector<std::vector<int>> move_area = btl_fld.find_move_area(max_character->get_current_cell(), max_character->get_mv_range());
        std::vector<int> neighbors_attack = btl_fld.area_in_area(move_area, btl_fld.search_neighbors(enemy_cell));
        if (btl_fld.is_in_area({neighbors_attack}, max_character->get_current_cell())) {
            max_character->do_damage(btl_fld.get_character_from_id(enemy_cell));
            return true;
        }
        std::vector<int> route = btl_fld.find_route(neighbors_attack[0], move_area); // строим путь к клетке
        btl_fld.add_highlight_cells(route, color_trace, color_trace);
        max_character->move(route, btl_fld);
        btl_fld.update_cell(max_character, neighbors_attack[0]);
        max_character->do_damage(btl_fld.get_character_from_id(enemy_cell));
        return true;
    }

    int char_index = rand() % get_chars_size();
    int cell_char = chars[char_index]->get_current_cell();
    std::vector<std::vector<int>> move_area = btl_fld.find_move_area(cell_char, chars[char_index]->get_mv_range());
    int move_cell = move_area[move_area.size() - 1][rand() % move_area[move_area.size() - 1].size()];
    std::vector<int> route = btl_fld.find_route(move_cell, move_area);
    btl_fld.add_highlight_cells(route, color_trace, color_trace);
    chars[char_index]->move(route, btl_fld);
    btl_fld.update_cell(chars[char_index], move_cell);
    return true;
}
