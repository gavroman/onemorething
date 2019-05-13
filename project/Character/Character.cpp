//
// Created by arugaf on 13.05.19.
//

#include "Character.h"

Scout::Scout() {
    idle_texture.loadFromFile("../source/characters/scout/1/Scout_idle.png");
    idle_texture.setSmooth(true);
    sprite.setTexture(idle_texture);
    status = IDLE;
    texture_x = 0;
    texture_y = 0;
    texture_height = 776;
    texture_width = 590;
}

void Character::animate() {
    switch(status) {
        case IDLE: {
            sprite.setTextureRect(sf::Rect(texture_x, texture_y, texture_width, texture_height));
            texture_x += texture_width;
            if (texture_x >= texture_width * 4) {
                texture_x = 0;
            }
        }
        case WALK: {

        }
    }
}

void Character::draw(sf::RenderWindow& window) {
    this->animate();
    window.draw(this->sprite);
}