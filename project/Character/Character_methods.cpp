//
// Created by arugaf on 19.05.19.
//

#include "Character.h"

/* В этом файле содержатся методы персонажей */

int Character::get_heal() {
    return heal;
}

bool Character::get_range() {
    return range;
}

void Character::set_attack_target(std::shared_ptr<Character> character) {
    attack_target = character;
}

void Character::do_damage(std::shared_ptr<Character> character) {
    character->hp -= (rand () % (damage_max - damage_min)) + damage_min;
    /*if (sprite.getPosition().x > character->sprite.getPosition().x and !inverse) {
        inverse = true;
        sprite.setOrigin(texture_width, 0);
        sprite.setScale(-scale, scale);
    } else if (sprite.getPosition().x < character->sprite.getPosition().x and inverse) {
        inverse = false;
        sprite.setOrigin(0, 0);
        sprite.setScale(scale, scale);
    }*/
    status = ATTACK;
    texture_x = 0;

    if ((get_current_cell() / 32) % 2 == 1) {
        if ((get_current_cell() % 2 == 1 && (character->get_current_cell() == get_current_cell() + 1 || character->get_current_cell() % 2 == 1))
            || (get_current_cell() % 2 == 0 && (character->get_current_cell() == get_current_cell() + 1 || character->get_current_cell() % 2 == 0))) {
            inverse = false;
        } else if ((get_current_cell() % 2 == 1 && character->get_current_cell() % 2 == 0) || (get_current_cell() % 2 == 0 && character->get_current_cell() % 2 == 1)) {
            inverse = true;
        }
    } else if ((get_current_cell() / 32) % 2 == 0) {
        if ((get_current_cell() % 2 == 1 && (character->get_current_cell() == get_current_cell() - 1 || character->get_current_cell() % 2 == 1))
            || (get_current_cell() % 2 == 0 && (character->get_current_cell() == get_current_cell() - 1 || character->get_current_cell() % 2 == 0))) {
            inverse = true;
        } else if ((get_current_cell() % 2 == 1 && character->get_current_cell() % 2 == 0) || (get_current_cell() % 2 == 0 && character->get_current_cell() % 2 == 1)) {
            inverse = false;
        }
    }

    /*if ((get_current_cell() % 2 == 1 && (character->get_current_cell() == get_current_cell() + 1 || character->get_current_cell() % 2 == 1)) || (get_current_cell() % 2 == 0 && (character->get_current_cell() == get_current_cell() + 1 || character->get_current_cell() % 2 == 0))) inverse = false;
    else if ((get_current_cell() % 2 == 1 && character->get_current_cell() % 2 == 0) || (get_current_cell() % 2 == 0 && character->get_current_cell() % 2 == 1)) inverse = true;
*/
    /*if ((get_current_cell() % 2 == 1 && (character->get_current_cell() == get_current_cell() + 1 || character->get_current_cell() % 2 == 1)) ||
        (get_current_cell() % 2 == 0 && (character->get_current_cell() == get_current_cell() + 1 || character->get_current_cell() % 2 == 0))) {
        inverse = false;

    } else if ((get_current_cell() % 2 == 1 && character->get_current_cell() % 2 == 0) ||
              (get_current_cell() % 2 == 0 && character->get_current_cell() % 2 == 1)) {
        inverse = true;
    }*/

    if (inverse) {
        sprite.setScale(-scale, scale);
        sprite.setOrigin(reverse_offset + attack_offset_x, idle_walk_offset_y + attack_offset_y);
    } else {
        sprite.setScale(scale, scale);
        sprite.setOrigin(idle_walk_offset_x + attack_offset_x, idle_walk_offset_y + attack_offset_y);
    }

    character->status = HURT;

    if (character->inverse) {
        character->sprite.setScale(-scale, scale);
        character->sprite.setOrigin(character->reverse_offset + character->hurt_offset_x, character->idle_walk_offset_y + character->hurt_offset_y);
    } else {
        character->sprite.setScale(scale, scale);
        character->sprite.setOrigin(character->idle_walk_offset_x + character->hurt_offset_x, character->idle_walk_offset_y + character->hurt_offset_y);
    }

    character->texture_x = 0;
}

void Character::do_heal(std::shared_ptr<Character> character) {
    if (character->max_hp - character->hp < heal) {
        character->hp = character->max_hp;
    } else {
        character->hp += heal;
    }
    status = HEAL;
}

int Character::get_hp() {
    return hp;
}

int Character::get_max_hp() {
    return max_hp;
}

int Character::get_max_damage() {
    return damage_max;
}

bool Character::get_inverse() {
    return inverse;
}

void Character::animate() {
    switch(status) {
        case IDLE: {
            sprite.setTexture(idle_texture);
            texture_width = idle_texture.getSize().x / sprites_amount;
            texture_height = idle_texture.getSize().y;
            sprite.setTextureRect(sf::Rect(texture_x, texture_y, texture_width, texture_height));
            if (texture_x >= texture_width * (sprites_amount - 1)) {
                texture_x = 0;
            } else {
                texture_x += texture_width;
            }
            break;
        }

        case WALK: {
            sprite.setTexture(walk_texture);
            texture_width = walk_texture.getSize().x / sprites_amount;
            texture_height = walk_texture.getSize().y;
            sprite.setTextureRect(sf::Rect(texture_x, texture_y, texture_width, texture_height));
            if (texture_x >= texture_width * (sprites_amount - 1)) {
                texture_x = 0;
            } else {
                texture_x += texture_width;
            }
            sprite.setPosition(animate_positions[current_animate_index]);
            if (++current_animate_index == animate_positions.size() - 1) {
                if(attack_target) {//если в конце маршрута стоит вражеский перс, надо пиздануть
                    do_damage(attack_target);
                    attack_target = nullptr;
                } else {
                    status = IDLE;
                    texture_x = 0;
                }  
                current_animate_index = 0; 
            }
            if (current_animate_index != 0) {
                if (animate_positions[current_animate_index].x - animate_positions[current_animate_index - 1].x < 0) {
                    inverse = true;
                    sprite.setScale(-scale, scale);
                    sprite.setOrigin(reverse_offset, idle_walk_offset_y);
                    //std::cout << texture_width << std::endl << idle_walk_offset_x << std::endl;
                } else {
                    inverse = false;
                    sprite.setScale(scale, scale);
                    sprite.setOrigin(idle_walk_offset_x, idle_walk_offset_y);
                }
            }
            break;
        }

        case ATTACK: {
            sprite.setTexture(attack_texture);
            texture_width = attack_texture.getSize().x / sprites_amount;
            texture_height = attack_texture.getSize().y;
            sprite.setTextureRect(sf::Rect(texture_x, texture_y, texture_width, texture_height));
            if (texture_x >= texture_width * (sprites_amount - 1)) {
                texture_x = 0;
            } else {
                texture_x += texture_width;
            }
            if (current_animate_index == sprites_amount) {
                current_animate_index = 0;
                status = IDLE;
                texture_x = 0;
                if (inverse) {
                    sprite.setScale(-scale, scale);
                    sprite.setOrigin(reverse_offset, idle_walk_offset_y);
                } else {
                    sprite.setScale(scale, scale);
                    sprite.setOrigin(idle_walk_offset_x, idle_walk_offset_y);
                }
            } else {
                current_animate_index++;
            }    
            break;
        }

        case HURT: {
            sprite.setTexture(hurt_texture);
            texture_width = hurt_texture.getSize().x / sprites_amount;
            texture_height = hurt_texture.getSize().y;
            sprite.setTextureRect(sf::Rect(texture_x, texture_y, texture_width, texture_height));
            if (texture_x >= texture_width * (sprites_amount - 1)) {
                texture_x = 0;
            } else {
                texture_x += texture_width;
            }
            if (current_animate_index == sprites_amount) {
                current_animate_index = 0;
                status = IDLE;
                texture_x = 0;
                if (inverse) {
                    sprite.setScale(-scale, scale);
                    sprite.setOrigin(reverse_offset, idle_walk_offset_y);
                } else {
                    sprite.setScale(scale, scale);
                    sprite.setOrigin(idle_walk_offset_x, idle_walk_offset_y);
                }
            } else {
                current_animate_index++;
            }
            break;
        }

        case DYING: {
            sprite.setTexture(die_texture);
            texture_width = die_texture.getSize().x / sprites_amount;
            texture_height = die_texture.getSize().y;
            sprite.setTextureRect(sf::Rect(texture_x, texture_y, texture_width, texture_height));
            if (texture_x >= texture_width * (sprites_amount - 1)) {
                texture_x = 0;
            } else {
                texture_x += texture_width;
            }
            break;
        }

        case DEAD: {
            sprite.setTexture(die_texture);
            texture_width = die_texture.getSize().x / sprites_amount;
            texture_height = die_texture.getSize().y;
            sprite.setTextureRect(sf::Rect(texture_x * (sprites_amount - 1), texture_y, texture_width, texture_height));
            break;
        }

        case HEAL: {
            sprite.setTexture(heal_texture);
            texture_width = heal_texture.getSize().x / sprites_amount;
            texture_height = heal_texture.getSize().y;
            sprite.setTextureRect(sf::Rect(texture_x, texture_y, texture_width, texture_height));
            if (texture_x >= texture_width * (sprites_amount - 1)) {
                texture_x = 0;
            } else {
                texture_x += texture_width;
            }
            if (current_animate_index == sprites_amount) {
                current_animate_index = 0;
                status = IDLE;
            } else {
                current_animate_index++;
            }
            break;
        }
    }
}

void Character::move(std::vector<int> way, class Map field) {
    status = WALK;
    texture_x = 0;
    std::reverse(way.begin(), way.end());
    std::vector<sf::Vector2f> positions;
    for (int i = 0; i != way.size() - 1 ; i++) {
        std::vector<sf::Vector2f> discr_poses = field.discrete_positions(way[i], way[i+1], animation_steps);
        for (auto& discr_pos : discr_poses) {
            discr_pos.x /*-= idle_walk_offset_x*/;
            discr_pos.y /*-= idle_walk_offset_y*/;
            positions.emplace_back(discr_pos);
        }
    }
    positions.emplace_back(field.get_cell_pos(way[way.size() - 1]));
    animate_positions = positions;
}

void Character::draw(sf::RenderWindow& window, class Map field) {
    if (status != WALK) {
        sf::Vector2f pos = field.get_cell_pos(cell_id);
        sprite.setPosition(pos.x /*- idle_walk_offset_x*/, pos.y /*- idle_walk_offset_y*/);
    }
    window.draw(sprite);
    sf::Font font;
    font.loadFromFile("../source/menu/Enchanted_Land.otf");
    sf::Text hp_text(std::to_string(hp), font, 30);
    hp_text.setFillColor(sf::Color::Red);
    hp_text.setPosition(sprite.getPosition().x, sprite.getPosition().y - 20);
    window.draw(hp_text);
}

int Character::get_current_cell() {
    return cell_id;
}

int Character::get_mv_range() {
    return move_range;
}

std::vector<std::vector<int>> Character::get_move_area() {
    return move_area;
}

int Character::get_status() {
    return status;
}

void Character::update_id(const int id) {
    cell_id = id;
}

void Character::set_active(const bool active_stmt) {
    active = active_stmt;
}

void Character::set_move_area(std::vector<std::vector<int>>& area) {
    move_area = area;
}

void Character::set_route(std::vector<int>& way) {
    route = way;
}

bool Character::is_active() {
    return active;
}

bool Character::is_idle() {
    return status == IDLE;
}
