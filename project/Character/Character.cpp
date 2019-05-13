//
// Created by arugaf on 13.05.19.
//

#include "Character.h"

int Character::get_current_cell() {
	std::cout << "ZALUPA" << std::endl;
	std::cout << cell_id << std::endl;
	std::cout << "ZALUPA" << std::endl;
	return 1;
}

Scout::Scout() {
	cell_id = 0;	
	std::cout << cell_id << std::endl;
    idle_texture.loadFromFile("../source/characters/scout/1/1_IDLE_000.png");
    idle_texture.setSmooth(true);
    sprite.setTexture(idle_texture);
    sprite.setScale(sf::Vector2f(scale, scale));
}
