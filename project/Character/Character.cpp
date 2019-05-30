//
// Created by arugaf on 13.05.19.
//
#include "Character.h"

/* В этом файле содержатся конструкторы персонажей */

// TODO: Общее: смещение при hurt

Character::Character() {
    status = IDLE;

    idle_texture.setSmooth(true);
    walk_texture.setSmooth(true);
    hurt_texture.setSmooth(true);
    attack_texture.setSmooth(true);
    die_texture.setSmooth(true);

    texture_x = 0;
    texture_y = 0;
}

Scout::Scout(const int id, const int current_player) {
    std::string PLAYER;
    switch (current_player) {
        case PLAYER1: {
            PLAYER = "PLAYER1";
            inverse = false;

            attack_offset_x = 4;
            attack_offset_y = 5;
            hurt_offset_x = 8;
            hurt_offset_y = 3;

            break;
        }
        case PLAYER2: {
            PLAYER = "PLAYER2";
            inverse = true;

            attack_offset_x = 1;
            attack_offset_y = 2;
            hurt_offset_x = 8;
            hurt_offset_y = 4;

            break;
        }
    }
    std::string class_name = "scout/";
    std::string path = SOURCE + class_name + PLAYER;

    move_range = 30; //8
    cell_id = id;
    idle_walk_offset_x = 6;
    idle_walk_offset_y = 30;
    hp = 3000; //300
    damage_min = 50; //50
    damage_max = 55; //55
    reverse_offset = 63;

    idle_texture.loadFromFile(path + "/idle.png");
    walk_texture.loadFromFile(path + "/walk.png");
    hurt_texture.loadFromFile(path + "/hurt.png");
    attack_texture.loadFromFile(path + "/attack.png");
    die_texture.loadFromFile(path + "/die.png");

    sprites_amount = 5;
    scale = 1;

    sprite.setScale(sf::Vector2f(scale, scale));
    sprite.setOrigin(idle_walk_offset_x, idle_walk_offset_y);

    // TODO: Баг с мерцанием (у PLAYER1), попадает часть плаща в attack у PLAYER2

    if (inverse) {
        sprite.setScale(-scale, scale);
        sprite.setOrigin(reverse_offset, idle_walk_offset_y);
    }
}

Archer::Archer(const int id, const int current_player) {
    std::string PLAYER;
    switch (current_player) {
        case PLAYER1: {
            PLAYER = "PLAYER1";
            inverse = false;
            break;
        }
        case PLAYER2: {
            PLAYER = "PLAYER2";
            inverse = true;
            break;
        }
    }
    std::string class_name = "archer/";
    std::string path = SOURCE + class_name + PLAYER;

    move_range = 3;
    cell_id = id;
    idle_walk_offset_x = 10;
    idle_walk_offset_y = 40;
    attack_offset_x = 0;
    attack_offset_y = 0;
    hurt_offset_x = 6;
    hurt_offset_y = 3;
    hp = 250;
    damage_min = 15;
    damage_max = 20;
    reverse_offset = 70;

    idle_texture.loadFromFile(path + "/idle.png");
    walk_texture.loadFromFile(path + "/walk.png");
    hurt_texture.loadFromFile(path + "/hurt.png");
    attack_texture.loadFromFile(path + "/attack.png");
    die_texture.loadFromFile(path + "/die.png");

    sprites_amount = 5;
    scale = 1;

    sprite.setScale(sf::Vector2f(scale, scale));
    sprite.setOrigin(idle_walk_offset_x, idle_walk_offset_y);

    // TODO: Баг с мерцанием (у PLAYER2), смещение по оси y у idle (PLAYER2), смещение по оси x у idle (PLAYER1), какой-то трэш при walk (попадают части текстур, схлопывается текстурка PLAYER1)

    if (inverse) {
        sprite.setScale(-scale, scale);
        sprite.setOrigin(reverse_offset, idle_walk_offset_y);
    }
}

Swordman::Swordman(const int id, const int current_player) {
    std::string PLAYER;
    switch (current_player) {
        case PLAYER1: {
            PLAYER = "PLAYER1";

            idle_walk_offset_x = 6;
            idle_walk_offset_y = 40;

            scale = 0.085;

            inverse = false;
            break;
        }
        case PLAYER2: {
            PLAYER = "PLAYER2";

            idle_walk_offset_x = 5;
            idle_walk_offset_y = 38;

            scale = 0.21;

            inverse = true;
            break;
        }
    }
    std::string class_name = "swordman/";
    std::string path = SOURCE + class_name + PLAYER;

    move_range = 5;
    cell_id = id;
    hp = 500;
    damage_min = 70;
    damage_max = 130;

    idle_texture.loadFromFile(path + "/idle.png");
    walk_texture.loadFromFile(path + "/walk.png");
    hurt_texture.loadFromFile(path + "/hurt.png");
    attack_texture.loadFromFile(path + "/attack.png");
    die_texture.loadFromFile(path + "/die.png");

    sprites_amount = 5;
    sprite.setScale(sf::Vector2f(scale, scale));

    //TODO: Баг с мерцанием (у PLAYER2), попадают части текстуры в walk (у PLAYER2)
}

Tank::Tank(const int id, const int current_player) {
    std::string PLAYER;
    switch (current_player) {
        case PLAYER1: {
            PLAYER = "PLAYER1";
            inverse = false;
            break;
        }
        case PLAYER2: {
            PLAYER = "PLAYER2";
            inverse = true;
            break;
        }
    }
    std::string class_name = "tank/";
    std::string path = SOURCE + class_name + PLAYER;

    move_range = 3;
    cell_id = id;
    idle_walk_offset_x = 30;
    idle_walk_offset_y = 440;
    hp = 800;
    damage_min = 60;
    damage_max = 80;

    idle_texture.loadFromFile(path + "/idle.png");
    walk_texture.loadFromFile(path + "/walk.png");
    hurt_texture.loadFromFile(path + "/hurt.png");
    attack_texture.loadFromFile(path + "/attack.png");
    die_texture.loadFromFile(path + "/die.png");

    sprites_amount = 7;
    scale = 0.09;

    sprite.setScale(sf::Vector2f(scale, scale));
    sprite.setOrigin(idle_walk_offset_x, idle_walk_offset_y);

    // TODO: Баг с мерцанием (PLAYER1, PLAYER2), опять трэш в walk (попадают части текстуры, неверная центровка при повороте для PLAYER1 и PLAYER2), сильное смещение при attack и die (PLAYER1, PLAYER2)
}

Wizard::Wizard(const int id, const int current_player) {
    std::string PLAYER;
    switch (current_player) {
        case PLAYER1: {
            PLAYER = "PLAYER1";
            inverse = false;
            break;
        }
        case PLAYER2: {
            PLAYER = "PLAYER2";
            inverse = true;
            break;
        }
    }
    std::string class_name = "wizard/";
    std::string path = SOURCE + class_name + PLAYER;

    move_range = 3;
    cell_id = id;
    idle_walk_offset_x = 10;
    idle_walk_offset_y = 40;
    hp = 250;
    damage_min = 18;
    damage_max = 27;

    idle_texture.loadFromFile(path + "/idle.png");
    walk_texture.loadFromFile(path + "/walk.png");
    hurt_texture.loadFromFile(path + "/hurt.png");
    attack_texture.loadFromFile(path + "/attack.png");
    die_texture.loadFromFile(path + "/die.png");

    sprites_amount = 5;
    scale = 0.22;

    sprite.setScale(sf::Vector2f(scale, scale));

    // TODO: Баг с мерцанием (PLAYER1, PLAYER2), трэш в walk (PLAYER1)
}

Berserker::Berserker(const int id, const int current_player) {
    std::string PLAYER;
    switch (current_player) {
        case PLAYER1: {
            PLAYER = "PLAYER1";
            inverse = false;
            break;
        }
        case PLAYER2: {
            PLAYER = "PLAYER2";
            inverse = true;
            break;
        }
    }
    std::string class_name = "berserker/";
    std::string path = SOURCE + class_name + PLAYER;

    move_range = 6;
    cell_id = id;
    idle_walk_offset_x = 4;
    idle_walk_offset_y = 45;
    hp = 400;
    damage_min = 130;
    damage_max = 200;

    idle_texture.loadFromFile(path + "/idle.png");
    walk_texture.loadFromFile(path + "/walk.png");
    hurt_texture.loadFromFile(path + "/hurt.png");
    attack_texture.loadFromFile(path + "/attack.png");
    die_texture.loadFromFile(path + "/die.png");

    sprites_amount = 5;
    scale = 0.09;

    sprite.setScale(sf::Vector2f(scale, scale));

    // TODO: Баг с мерцанием (PLAYER1, PLAYER2), трэш в walk (залезает часть текстур для PLAYER2), смещение в walk (PLAYER2)
}

Knight::Knight(const int id, const int current_player) {
    std::string PLAYER;
    switch (current_player) {
        case PLAYER1: {
            PLAYER = "PLAYER1";
            idle_walk_offset_x = 10;
            idle_walk_offset_y = 38;
            inverse = false;
            break;
        }
        case PLAYER2: {
            PLAYER = "PLAYER2";
            idle_walk_offset_x = 5;
            idle_walk_offset_y = 50;
            inverse = true;
            break;
        }
    }
    std::string class_name = "knight/";
    std::string path = SOURCE + class_name + PLAYER;

    move_range = 4;
    cell_id = id;
    hp = 600;
    damage_min = 80;
    damage_max = 100;

    idle_texture.loadFromFile(path + "/idle.png");
    walk_texture.loadFromFile(path + "/walk.png");
    hurt_texture.loadFromFile(path + "/hurt.png");
    attack_texture.loadFromFile(path + "/attack.png");
    die_texture.loadFromFile(path + "/die.png");

    sprites_amount = 5;
    scale = 0.09;

    sprite.setScale(sf::Vector2f(scale, scale));

    // TODO: Баг с мерцанием (PLAYER1, PLAYER2), попадает часть текстур и смещение по x у hurt (PLAYER1), заметная разница в attack по y у die (PLAYER1)
}

Healer::Healer(const int id, const int current_player) {
    std::string PLAYER;
    switch (current_player) {
        case PLAYER1: {
            PLAYER = "PLAYER1";
            inverse = false;
            break;
        }
        case PLAYER2: {
            PLAYER = "PLAYER2";
            inverse = true;
            break;
        }
    }

    std::string class_name = "healer/";

    std::string path = SOURCE + class_name + PLAYER;

    move_range = 4;
    cell_id = id;
    idle_walk_offset_x = 4;
    idle_walk_offset_y = 35;
    active = false;
    status = IDLE;
    hp = 250;
    damage_min = 30;
    damage_max = 35;

    idle_texture.loadFromFile(path + "/idle.png");
    walk_texture.loadFromFile(path + "/walk.png");
    hurt_texture.loadFromFile(path + "/hurt.png");
    attack_texture.loadFromFile(path + "/attack.png");
    die_texture.loadFromFile(path + "/die.png");
    heal_texture.loadFromFile(path + "/heal.png");
    heal_texture.setSmooth(true);

    sprites_amount = 5;
    scale = 0.11;

    sprite.setScale(sf::Vector2f(scale, scale));

    // TODO: Баг с мерцанием (PLAYER1, PLAYER2), трэш в walk (PLAYER1, PLAYER2), сильное смещение в attack по y (PLAYER1, PLAYER2)
}


