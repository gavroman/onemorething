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

Human::Human(class Map field) {
    //chars.push_back(std::make_shared<Scout>(11, PLAYER1));
    //chars.push_back(std::make_shared<Archer>(12, PLAYER1));
    //chars.push_back(std::make_shared<Healer>(13, PLAYER1));
    //chars.push_back(std::make_shared<Knight>(14, PLAYER1));
    chars.push_back(std::make_shared<Swordman>(15, PLAYER1));
    //chars.push_back(std::make_shared<Tank>(16, PLAYER1));
    //chars.push_back(std::make_shared<Wizard>(17, PLAYER1));
    chars.push_back(std::make_shared<Berserker>(18, PLAYER1));

    for (auto &chr : chars) {
    	field.update_cell(chr, chr->get_current_cell());
    }
    deactivate_all_chars();
}

bool Human::make_turn(class Map& btl_fld, sf::Event event, sf::RenderWindow& window) {
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
                    deactivate_all_chars();
                    chars[char_index]->set_active(true);
                    btl_fld.drop_highlight_cells();
                    btl_fld.add_highlight_cells(move_area[chars[char_index]->get_mv_range()], color, color);
                    btl_fld.add_highlight_cells({move_area[0][0]}, color_trace, color_trace);   
                } else {
                    int active_char_index = get_active_char_index();
                    if (active_char_index != -1) {
                        std::vector<std::vector<int>> move_area = chars[active_char_index]->get_move_area();
                        if (btl_fld.is_in_area(move_area, cell_id)) {
                            std::vector<int> route = btl_fld.find_route(cell_id, move_area);
                            btl_fld.drop_highlight_cells();
                            btl_fld.add_highlight_cells(route, color_trace, color_trace);
                            chars[active_char_index]->move(route, btl_fld);  //НЕРАБОЧЕЕ ГАВНО
                            btl_fld.update_cell(chars[active_char_index], route[0]);
                            chars[active_char_index]->set_active(false);
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
            break;   
    }
    return false;
}



Bot::Bot(class Map field) {
    //chars.push_back(std::make_shared<Scout>(110, PLAYER2));
    //chars.push_back(std::make_shared<Archer>(111, PLAYER2));
    //chars.push_back(std::make_shared<Healer>(112, PLAYER2));
    //chars.push_back(std::make_shared<Knight>(113, PLAYER2));
    chars.push_back(std::make_shared<Swordman>(114, PLAYER2));
    //chars.push_back(std::make_shared<Tank>(115, PLAYER2));
    //chars.push_back(std::make_shared<Wizard>(116, PLAYER2));
    chars.push_back(std::make_shared<Berserker>(117, PLAYER1));

    for (auto &chr : chars) {
    	field.update_cell(chr, chr->get_current_cell());
    }
}

bool Bot::make_turn(class Map& btl_fld, sf::Event event, sf::RenderWindow& window) {
    //TODO (9rik): Ярик ебашь
    switch(event.type) {
        case sf::Event::Closed: {
            window.close();
            break;
        }

        case sf::Event::KeyPressed: { // окончание хода по нажатию кнопки A
            if (event.key.code == sf::Keyboard::A) {
                return true;
            }
        }
    }   
    return false; 
}

