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
                status = IDLE;
            }
            break;
        }
    }
}

void Character::move(std::vector<int> way, class Map field) {
    status = WALK;

    std::reverse(way.begin(), way.end());
    std::cout << "-------poses from way-------" << std::endl;
    for (int i = 0; i != way.size(); i++) {
        std::cout << "id = " << way[i] <<") " << field.get_cell_pos(way[i]).x << "   " << field.get_cell_pos(way[i]).y <<std::endl;
    }
    std::cout << "----------------------------" << std::endl;
    std::cout << "________________________________________________" << std::endl;
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
    for (int i = 0; i != positions.size(); i++) {
        std::cout << " pos = " << positions[i].x << "   " << positions[i].y << std::endl;
    }

    std::cout << "________________________________________________" << std::endl;
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

Scout::Scout(const int id) {
    move_range = 4;
	cell_id = id;
    map_offset_x = 6;
    map_offset_y = 35;

    status = IDLE;
    

    idle_texture.loadFromFile("../source/characters/scout/PLAYER2/idle.png");
    walk_texture.loadFromFile("../source/characters/scout/PLAYER2/walk.png");
    idle_texture.setSmooth(true);
    walk_texture.setSmooth(true);
    texture_x = 0;
    texture_y = 0;
    texture_height = 776;
    texture_width = 590;

    sprite.setTexture(idle_texture);
    sprite.setTextureRect(sf::Rect(texture_x, texture_y, texture_width, texture_height));
    sprite.setScale(sf::Vector2f(scale, scale));    
}

