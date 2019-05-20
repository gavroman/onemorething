//
// Created by arugaf on 19.05.19.
//

#include "Character.h"

/* В этом файле содержатся методы персонажей */

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
            if (++current_animate_index == animate_positions.size()) {
                current_animate_index = 0;
                status = IDLE;
            }
            if (current_animate_index != 0) {
                if (animate_positions[current_animate_index].x - animate_positions[current_animate_index - 1].x < 0) {
                    inverse = true;
                    sprite.setScale(-scale, scale);
                    sprite.setOrigin(texture_width, 0);
                } else {
                    inverse = false;
                    sprite.setScale(scale, scale);
                    sprite.setOrigin(0, 0);
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
    }
}

void Character::move(std::vector<int> way, class Map field) {
    status = WALK;
    std::reverse(way.begin(), way.end());
    std::vector<sf::Vector2f> positions;
    for (int i = 0; i != way.size() - 1 ; i++) {
        std::vector<sf::Vector2f> discr_poses = field.discrete_positions(way[i], way[i+1], animation_steps);
        for (auto& discr_pos : discr_poses) {
            discr_pos.x -= map_offset_x;
            discr_pos.y -= map_offset_y;
            positions.emplace_back(discr_pos);
        }
    }
    positions.emplace_back(field.get_cell_pos(way[way.size() - 1]));
    animate_positions = positions;
}

void Character::draw(sf::RenderWindow& window, class Map field) {
    if (status != WALK) {
        sf::Vector2f pos = field.get_cell_pos(cell_id);
        sprite.setPosition(pos.x - map_offset_x, pos.y - map_offset_y);
    }
    window.draw(sprite);
}

int Character::get_current_cell() {
    return cell_id;
}

int Character::get_mv_range() {
    return move_range;
}

void Character::update_id(const int id) {
    cell_id = id;
}

void Character::set_active(const bool active_stmt) {
    active = active_stmt;
}

bool Character::is_active() {
    return active;
}

bool Character::is_idle() {
    return status == IDLE;
}