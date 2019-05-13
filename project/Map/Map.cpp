//
// Created by arugaf on 08.05.19.
//

#include "Map.h"
#include "tinyxml2.h"

/* В этом файле содержится все, что касается самой карты.
 * Само создание, отрисовка и т.п. */

Map::Map(const std::string xml_file)
        : xml_file_path(std::move(xml_file)) {
    using namespace tinyxml2;
    XMLDocument doc;
    if (doc.LoadFile(xml_file_path.c_str())) {
        return;
    }
    XMLElement * map_xml = doc.FirstChildElement("map");
    if (map_xml == nullptr) {
        return;
    }
    map_size_width = map_xml->IntAttribute("width", 0);
    map_size_height = map_xml->IntAttribute("height", 0);
    hex_size_width = map_xml->IntAttribute("tilewidth", 0);
    hex_size_height = map_xml->IntAttribute("tileheight", 0);

    std::string tileset_tsx = map_xml->FirstChildElement("tileset")->Attribute("source");
    XMLDocument doc_tileset;
    tileset_tsx = "../source/game_map/" + tileset_tsx;
    if (doc_tileset.LoadFile(tileset_tsx.c_str())) {
        return;
    }
    XMLElement * tileset_xml = doc_tileset.FirstChildElement("tileset");
    int sprite_width = tileset_xml->IntAttribute("tilewidth", 0);
    int sprite_height = tileset_xml->IntAttribute("tileheight", 0);
    int sprite_count = tileset_xml->IntAttribute("tilecount", 0);
    int sprite_columns = tileset_xml->IntAttribute("columns", 0);

    std::string img_src = tileset_xml->FirstChildElement("image")->Attribute("source");
    img_src = "../source/game_map/" + img_src.erase(0,3);
    if(!map_texture.loadFromFile(img_src)) {
        return;
    }
    map_texture.setSmooth(true);
    std::vector<sf::Sprite> hex_sprites;
    int current_x = 0;
    int current_y = 0;
    for (int i = 0; i != sprite_count; i++) {
        if (!(i % sprite_columns) && i) {
            current_y += sprite_height;
            current_x = 0;
        }
        hex_sprites.emplace_back(sf::Sprite( map_texture, sf::Rect(current_x,
                                                                   current_y,
                                                                   sprite_width,
                                                                   sprite_height)));
        current_x += sprite_width;
    }
    int i = 0;
    XMLElement * tile_xml = map_xml->FirstChildElement("layer")
                                   ->FirstChildElement("data")
                                   ->FirstChildElement("tile");
    do {
        std::shared_ptr cell = std::make_shared<Cell>();
        cell->id = i++;
        cell->character = nullptr;
        cell->neighbors = search_neighbors(cell->id);
        
        int gid = tile_xml->IntAttribute("gid", 0);
        cell->passability = (gid <= 12); // Первые 12 тайлов проходимы

        cell->sprite = hex_sprites[gid - 1];
        sf::Vector2f sprite_pos = calculate_position(cell->id);
        cell->sprite.setPosition(sprite_pos);
        cell->sprite.scale(sf::Vector2f(scale, scale));
        cell->x = sprite_pos.x;
        cell->y = sprite_pos.y;

        map.emplace_back(cell);
    } while ((tile_xml = tile_xml->NextSiblingElement("tile")));

    //===============вывод id и id соседей===============
    /*for (auto& cell : map) {
        std::cout << "id: " << cell->id << " neighbors: ";
        for (auto& neighbor : cell->neighbors) {
            std::cout << neighbor << ", ";
        }
        std::cout << std::endl;
    }
    *///===================================================
}

sf::Vector2f Map::calculate_position(const int id) {
    int row = id / map_size_width;
    int col = id % map_size_width;

    float pos_x = col * hex_size_width + offset_x;
    float pos_y = row * (hex_size_height - hex_offset_y) + offset_y;

    if (row % 2 == 0) {
        pos_x += (float)hex_size_width / 2;
    }
    return sf::Vector2f(pos_x * scale, pos_y * scale);
}

std::vector<int> Map::search_neighbors(const int id) {
    std::vector<int> left_right{id - 1, id + 1};
    std::vector<int> upp {id - map_size_width - 1, id - map_size_width};
    std::vector<int> down {id + map_size_width - 1, id + map_size_width};
    std::vector<int> neighbors;
    if (id - (id % map_size_width) <= left_right[0]) {
        neighbors.push_back(left_right[0]);
    }
    if ((id + map_size_width - (id % map_size_width) - 1) >= left_right[1]) {
        neighbors.push_back(left_right[1]);
    }
    for (int i = 0; i < 2; i++) {
        if ((id - (id % map_size_width)) % (2 * map_size_width) == 0) {
            upp[i] += 1;
            down[i] += 1;
        }
        if (((id + map_size_width) % (2 * map_size_width) == 0) and (i == 0)) {
            continue;
        }
        if (((id + 1 + map_size_width) % (2 * map_size_width) == 0) and (i == 1)) {
            continue;
        }
        if (upp[i] >= 0) {
            neighbors.push_back(upp[i]);
        }
        if (down[i] < map_size_width * map_size_height) {
            neighbors.push_back(down[i]);
        }
    }
    return neighbors;
}

void Map::draw_map(sf::RenderWindow& window) {
    for (const auto& it : map) {
        window.draw((*it).sprite);
        if (it->character) {
            it->character->sprite.setPosition(it->sprite.getPosition());
            it->character->sprite.setScale(sf::Vector2f(scale * 0.3, scale * 0.3));
            window.draw(it->character->sprite);
        }
    }
}
