#include <random>

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
                break;
            }
        }
    }
    return can_attack_enemy;
}

bool Player::check_hp(class Map& field) {
    for (int i = 0; i < chars.size(); i++) {
        if (chars[i]->get_hp() <= 0) {
            field.update_cell(nullptr, chars[i]->get_current_cell());
            chars.erase(chars.begin() + i);
            break;
        }
    }
    if (chars.size()) {
        return false;
    } else {
        return true;
    }
}

std::vector<int> Player::get_my_chars(std::shared_ptr<Character> healer) {
    std::vector<int> my_chars;
    for (int i = 0; i < chars.size(); i++) {
        if (chars[i] != healer) {
            my_chars.push_back(chars[i]->get_current_cell());
        }
    }
    return my_chars;

}

std::shared_ptr<Character> Player::get_hiller() {
    for (int i = 0; i < chars.size(); i++) {
        if (chars[i]->get_heal()) {
            return chars[i];
        }
    }
    return nullptr;
}


int Player::find_nearest_cell(std::vector<std::vector<int>>  move_area, int enemy_cell, class Map& field) {
    int cell = move_area[0][0];
    float x1 = field.get_cell_pos(enemy_cell).x;
    float y1 = field.get_cell_pos(enemy_cell).y;
    for (int i = 0; i < move_area.size(); i++) {
        for (int j = 0; j < move_area[i].size(); j++) {
            float x2 = field.get_cell_pos(move_area[i][j]).x;
            float y2 = field.get_cell_pos(move_area[i][j]).y;
            float xcell = field.get_cell_pos(cell).x;
            float ycell = field.get_cell_pos(cell).y;
            if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) < (x1 - xcell) * (x1 - xcell) + (y1 - ycell) * (y1 - ycell)) {
                cell = move_area[i][j];
            }
        }
    }
    return cell;
}


Human::Human(class Map field, Current_player player, std::vector<int> characters) {
    std::vector<int> poses = {};
    if(player == PLAYER1) {
        poses = {416, 384, 352, 320, 288, 256, 224, 192};
    } else {
        poses = {31, 63, 95, 127, 159, 191, 223, 255};
    }
    int pos_index = 0;

    for (const auto& it : characters) {
        switch (it) {
            case 0: {
                chars.push_back(std::make_shared<Scout>(poses[pos_index++], player));
                std::cout << "Scout textures loaded" << std::endl;
                break;
            }
            case 1: {
                chars.push_back(std::make_shared<Archer>(poses[pos_index++], player));
                std::cout << "Archer textures loaded" << std::endl;
                break;
            }
            case 2: {
                chars.push_back(std::make_shared<Swordman>(poses[pos_index++], player));
                std::cout << "Swordman textures loaded" << std::endl;
                break;
            }
            case 3: {
                chars.push_back(std::make_shared<Tank>(poses[pos_index++], player));
                std::cout << "Tank textures loaded" << std::endl;
                break;
            }
            case 4: {
                chars.push_back(std::make_shared<Wizard>(poses[pos_index++], player));
                std::cout << "Wizard textures loaded" << std::endl;
                break;
            }
            case 5: {
                chars.push_back(std::make_shared<Berserker>(poses[pos_index++], player));
                std::cout << "Berserker textures loaded" << std::endl;
                break;
            }
            case 6: {
                chars.push_back(std::make_shared<Knight>(poses[pos_index++], player));
                std::cout << "Knight textures loaded" << std::endl;
                break;
            }
            case 7: {
                chars.push_back(std::make_shared<Healer>(poses[pos_index++], player));
                std::cout << "Healer textures loaded" << std::endl;
                break;
            }
        }
    }
    std::cout << "PLAYER1 done" << std::endl;
    for (auto &chr : chars) {
    	field.update_cell(chr, chr->get_current_cell());
    }
    deactivate_all_chars();
}

bool Human::make_turn(class Map& btl_fld, sf::RenderWindow& window) {
    if (check_hp(btl_fld)) {
        return true;
    }
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

                    if ((char_index != -1 and !status_heal) or (status_heal and cell_id == get_hiller()->get_current_cell())) {
                        //отрисовка зоны
                        std::vector<std::vector<int>> move_area = btl_fld.find_move_area(cell_id, chars[char_index]->get_mv_range());
                        chars[char_index]->set_move_area(move_area);
                        std::vector<int> attack_enemy;
                        if (chars[char_index]->get_range()) {
                            attack_enemy = get_enemy_chars(btl_fld);
                        } else {
                            attack_enemy = can_attack_chars(get_enemy_chars(btl_fld),
                                                            move_area[chars[char_index]->get_mv_range()],
                                                            btl_fld);
                        }
                        deactivate_all_chars();
                        chars[char_index]->set_active(true);
                        btl_fld.drop_highlight_cells();
                        btl_fld.add_highlight_cells(move_area[chars[char_index]->get_mv_range()], color, color);
                        if (chars[char_index]->get_heal()) {
                            std::vector<int> my_chars = can_attack_chars(get_my_chars(chars[char_index]),
                                                                         move_area[chars[char_index]->get_mv_range()],
                                                                         btl_fld);
                            btl_fld.add_highlight_cells(my_chars, color_my, color_my);
                            status_heal = true;
                        }
                        if (attack_enemy.size() != 0) {
                            btl_fld.add_highlight_cells(attack_enemy, color_enemy, color_enemy);
                        }
                        btl_fld.add_highlight_cells({move_area[0][0]}, color_trace, color_trace);   
                    } else {
                        int active_char_index = get_active_char_index();
                        if (active_char_index != -1) {
                            std::vector<std::vector<int>> move_area = chars[active_char_index]->get_move_area();
                            std::vector<int> attack_enemy;
                            if (chars[active_char_index]->get_range()) {
                                attack_enemy = get_enemy_chars(btl_fld);
                            } else {
                                attack_enemy = can_attack_chars(get_enemy_chars(btl_fld),
                                                                                 move_area[chars[active_char_index]->get_mv_range()],
                                                                                 btl_fld);
                            }

                            std::vector<int> neighbors_attack;
                            std::vector<int> my_chars;

                            if (chars[active_char_index]->get_heal()) {
                                my_chars = can_attack_chars(get_my_chars(chars[active_char_index]),
                                        move_area[chars[active_char_index]->get_mv_range()], btl_fld);
                                status_heal = false;

                            }

                            if (btl_fld.is_in_area({my_chars}, cell_id)) {
                                chars[active_char_index]->do_heal(btl_fld.get_character_from_id(cell_id));
                                chars[active_char_index]->set_active(false);
                                return true;
                            }
                            if (btl_fld.is_in_area({attack_enemy}, cell_id)) {
                                std::vector<int> neighbors_attack;
                                if (chars[active_char_index]->get_range()) {
                                    neighbors_attack = {chars[active_char_index]->get_current_cell()};
                                } else {
                                    neighbors_attack = btl_fld.area_in_area(move_area, btl_fld.search_neighbors(cell_id));
                                }
                                if (btl_fld.is_in_area({neighbors_attack}, chars[active_char_index]->get_current_cell())) {
                                    chars[active_char_index]->do_damage(btl_fld.get_character_from_id(cell_id));
                                    chars[active_char_index]->set_active(false);
                                    return true;
                                }
                                std::vector<int> route = btl_fld.find_route(neighbors_attack[0], move_area);
                                btl_fld.drop_highlight_cells();
                                btl_fld.add_highlight_cells(route, color_trace, color_trace);
                                chars[active_char_index]->move(route, btl_fld);
                                btl_fld.update_cell(chars[active_char_index], route[0]);
                                chars[active_char_index]->set_attack_target(btl_fld.get_character_from_id(cell_id));
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

            case sf::Event::KeyReleased: {
                switch (event.key.code) {
                    case sf::Keyboard::Escape: {
                        sf::Texture screenshot;
                        screenshot.loadFromImage(window.capture());
                        sf::Sprite screenshot_s(screenshot);
                        screenshot_s.setColor(sf::Color(255, 255, 255, 150));

                        for (int i = 0; i < 10; i++) {
                            std::mt19937 rand1(time(nullptr) + 20),
                                         rand2(time(nullptr) + 40),
                                         rand3(time(nullptr) + 60),
                                         rand4(time(nullptr) + 80);
                            screenshot_s.move(rand1() % 10, rand2() % 10);
                            window.draw(screenshot_s);
                            screenshot_s.setPosition(0, 0);

                            screenshot_s.move(- (rand3() % 10), - (rand4() % 10));
                            window.draw(screenshot_s);
                            screenshot_s.setPosition(0, 0);

                            window.draw(screenshot_s);
                        }

                        screenshot.loadFromImage(window.capture());
                        screenshot_s.setTexture(screenshot);

                        Pause_menu pause(window.getSize().x, window.getSize().y);
                        pause.process(&window, screenshot_s);
                        break;
                    }
                }
            }

            default: 
                return false;
            }
        }
    return false;
}

Bot::Bot(class Map field, Current_player player, std::vector<int> characters) {
    std::vector<int> poses = {};
    if(player == PLAYER1) {
        poses = {416, 384, 352, 320, 288, 256, 224, 192};
    } else {
        poses = {31, 63, 95, 127, 159, 191, 223, 255};
    }
    int pos_index = 0;
    for (const auto& it : characters) {
        switch (it) {
            case 0: {
                chars.push_back(std::make_shared<Scout>(poses[pos_index++], player));
                std::cout << "Scout textures loaded" << std::endl;
                break;
            }
            case 1: {
                chars.push_back(std::make_shared<Archer>(poses[pos_index++], player));
                std::cout << "Archer textures loaded" << std::endl;
                break;
            }
            case 2: {
                chars.push_back(std::make_shared<Swordman>(poses[pos_index++], player));
                std::cout << "Swordman textures loaded" << std::endl;
                break;
            }
            case 3: {
                chars.push_back(std::make_shared<Tank>(poses[pos_index++], player));
                std::cout << "Tank textures loaded" << std::endl;
                break;
            }
            case 4: {
                chars.push_back(std::make_shared<Wizard>(poses[pos_index++], player));
                std::cout << "Wizard textures loaded" << std::endl;
                break;
            }
            case 5: {
                chars.push_back(std::make_shared<Berserker>(poses[pos_index++], player));
                std::cout << "Berserker textures loaded" << std::endl;
                break;
            }
            case 6: {
                chars.push_back(std::make_shared<Knight>(poses[pos_index++], player));
                std::cout << "Knight textures loaded" << std::endl;
                break;
            }
            case 7: {
                chars.push_back(std::make_shared<Healer>(poses[pos_index++], player));
                std::cout << "Healer textures loaded" << std::endl;
                break;
            }
        }
    }
    std::cout << "PLAYER2 done" << std::endl;
    for (auto &chr : chars) {
    	field.update_cell(chr, chr->get_current_cell());
    }
}

bool Bot::make_turn(class Map& btl_fld, sf::RenderWindow& window) {
    if (check_hp(btl_fld)) {
        return true;
    }
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
    int max_damage = 0;
    std::shared_ptr<Character> max_character = nullptr;
    int enemy_cell = -1;

    for (int i = 0; i < chars.size(); i++) { // ищем перса который может ударить с max_атакой
        int cell_char = chars[i]->get_current_cell();
        std::vector<std::vector<int>> move_area = btl_fld.find_move_area(cell_char, chars[i]->get_mv_range());
        std::vector<int> attack_enemy;
        if (chars[i]->get_range()) {
            attack_enemy = get_enemy_chars(btl_fld);
        } else {
            attack_enemy = can_attack_chars(get_enemy_chars(btl_fld),
                                            move_area[chars[i]->get_mv_range()],
                                            btl_fld);
        }
        //std::vector<int> attack_enemy = can_attack_chars(get_enemy_chars(btl_fld),
         //                                                move_area[chars[i]->get_mv_range()], btl_fld);
        if (attack_enemy.size() and chars[i]->get_max_damage() > max_damage) {
            max_damage = chars[i]->get_max_damage();
            max_character = chars[i];
            enemy_cell = attack_enemy[0];
        }
    }

    std::shared_ptr<Character> healer = get_hiller();
    if (healer) {
        std::vector<std::vector<int>> move_area = btl_fld.find_move_area(healer->get_current_cell(), healer->get_mv_range());
        std::vector<int> my_chars = can_attack_chars(get_my_chars(healer), move_area[healer->get_mv_range()], btl_fld);
        int max_heal_index = -1;
        int max_heal = 0;
        for (int i = 0; i < my_chars.size(); i++) {
            if (chars[get_char_index_from_cell(my_chars[i])]->get_max_hp() - chars[get_char_index_from_cell(my_chars[i])]->get_hp() > max_heal) {
                max_heal = chars[get_char_index_from_cell(my_chars[i])]->get_max_hp() - chars[get_char_index_from_cell(my_chars[i])]->get_hp();
                max_heal_index = my_chars[i];
            }
        }
        if (max_heal > healer->get_heal()) {
            max_heal = healer->get_heal();
        }
        if (max_heal > max_damage) {
            healer->do_heal(chars[get_char_index_from_cell(max_heal_index)]);
            return true;
        }
    }

    if (max_character) {
        std::vector<std::vector<int>> move_area = btl_fld.find_move_area(max_character->get_current_cell(), max_character->get_mv_range());
        std::vector<int> neighbors_attack;
        if (max_character->get_range()) {
            neighbors_attack = {max_character->get_current_cell()};
        } else {
            neighbors_attack = btl_fld.area_in_area(move_area, btl_fld.search_neighbors(enemy_cell));
        }
        if (btl_fld.is_in_area({neighbors_attack}, max_character->get_current_cell())) {
            max_character->do_damage(btl_fld.get_character_from_id(enemy_cell));
            return true;
        }
        std::vector<int> route = btl_fld.find_route(neighbors_attack[0], move_area); // строим путь к клетке
        btl_fld.add_highlight_cells(route, color_trace, color_trace);
        max_character->move(route, btl_fld);
        btl_fld.update_cell(max_character, neighbors_attack[0]);
        max_character->set_attack_target(btl_fld.get_character_from_id(enemy_cell));
        return true;
    }
    std::vector<int> enemy_chars = get_enemy_chars(btl_fld);
    std::shared_ptr<Character> max_hirnyi = chars[0];
    for (int i = 1; i < chars.size(); i++) {
        if (max_hirnyi->get_hp() < chars[i]->get_hp()) {
            max_hirnyi = chars[i];
        }
    }

    int cell_char = max_hirnyi->get_current_cell();
    std::vector<std::vector<int>> move_area = btl_fld.find_move_area(cell_char, max_hirnyi->get_mv_range());
    int enemy_cell_char = find_nearest_cell({enemy_chars}, cell_char, btl_fld);
    std::cout << enemy_cell_char << std::endl;
    int move_cell = find_nearest_cell(move_area, enemy_cell_char, btl_fld);
    std::cout << move_cell << std::endl;
    std::vector<int> route = btl_fld.find_route(move_cell, move_area);
    btl_fld.add_highlight_cells(route, color_trace, color_trace);
    max_hirnyi->move(route, btl_fld);
    btl_fld.update_cell(max_hirnyi, move_cell);
    return true;
}
