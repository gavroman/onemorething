//
// Created by arugaf on 13.05.19.
//
#include "Map.h"

void Character::animate() {
    switch(status) {
        case IDLE: {
            sprite.setTexture(idle_texture);
            sprite.setTextureRect(sf::Rect(texture_x, texture_y, texture_width, texture_height));
            if (texture_x >= texture_width * 4) {
                texture_x = 0;
            } else {
                texture_x += texture_width;
            }
            break;
        }

        case WALK: {
            sprite.setTexture(walk_texture);
            sprite.setTextureRect(sf::Rect(texture_x, texture_y, texture_width, texture_height));
            if (texture_x >= texture_width * 4) {
                texture_x = 0;
            } else {
                texture_x += texture_width;
            }
            sprite.setPosition(animate_positions[current_animate_index]);    
            if (++current_animate_index == animate_positions.size()) {
                current_animate_index = 0;
                std::cout << "walk end" << std::endl;
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
    }
}

void Character::move(std::vector<int> way, class Map field) {
    animate_positions = {};
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

std::vector<std::vector<int>> Character::get_move_area() {
    return move_area;
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



Scout::Scout(const int id) {
    move_range = 4;
	cell_id = id;
    map_offset_x = 6;
    map_offset_y = 35;
    active = false;

    status = IDLE;

    idle_texture.loadFromFile("../source/characters/scout/PLAYER2/idle.png");
    walk_texture.loadFromFile("../source/characters/scout/PLAYER2/walk.png");
    idle_texture.setSmooth(true);
    walk_texture.setSmooth(true);
    texture_x = 0;
    texture_y = 0;
    texture_height = 776;
    texture_width = 590;
    scale = 0.11;

    sprite.setTexture(idle_texture);
    sprite.setTextureRect(sf::Rect(texture_x, texture_y, texture_width, texture_height));
    sprite.setScale(sf::Vector2f(scale, scale));    
}
