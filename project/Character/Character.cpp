//
// Created by arugaf on 13.05.19.
//

#include "Character.h"

Scout::Scout() {
    idle_texture.loadFromFile("../source/characters/scout/1/1_IDLE_000.png");
    idle_texture.setSmooth(true);
    sprite.setTexture(idle_texture);
}
