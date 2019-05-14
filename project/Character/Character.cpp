//
// Created by arugaf on 13.05.19.
//
#include "Map.h"

void Character::draw_character(sf::RenderWindow& window, class Map field) {
	std::cout << "id = " << cell_id <<std::endl;
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
    idle_texture.loadFromFile("../source/characters/scout/1/1_IDLE_000.png");
    idle_texture.setSmooth(true);
    sprite.setTexture(idle_texture);
    sprite.setScale(sf::Vector2f(scale, scale));    
}
