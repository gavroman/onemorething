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
            break;
        }
    }
}

void Character::draw_character(sf::RenderWindow& window, class Map field) {
	//sstd::cout << "id = " << cell_id <<std::endl;
	sf::Vector2f pos = field.get_cell_pos(cell_id);
    sprite.setPosition(pos.x - 6, pos.y - 35);
    window.draw(sprite);
}

int Character::get_current_cell() {
	return cell_id;
}

void Character::update_id(int id) {
	cell_id = id;
}


Scout::Scout(const int id) {

	cell_id = id;
    idle_texture.loadFromFile("../source/characters/scout/1/Scout_idle.png");
    walk_texture.loadFromFile("../source/characters/scout/1/Scout_walk.png");
    idle_texture.setSmooth(true);
    walk_texture.setSmooth(true);
    sprite.setTexture(idle_texture);
    sprite.setScale(sf::Vector2f(scale, scale));
    status = IDLE;
    texture_x = 0;
    texture_y = 0;
    texture_height = 776;
    texture_width = 590;

}

