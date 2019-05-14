//
// Created by arugaf on 13.05.19.
//

#include "Character.h"

void Character::draw_character(sf::RenderWindow& window) {
	window.draw(sprite);
}

int Character::get_current_cell() {
	return cell_id;
}

Scout::Scout(const int id, Map battle_field)
	: cell_id(id) {
	battle_field.update_cell(this, id);	
    idle_texture.loadFromFile("../source/characters/scout/1/1_IDLE_000.png");
    idle_texture.setSmooth(true);
    sprite.setTexture(idle_texture);
    sprite.setScale(sf::Vector2f(scale, scale));
}
