//
// Created by arugaf on 02.05.19.
//

#include "Map.h"

Map::Map(const std::string& xml_file_path) {
    xml_file.open(xml_file_path);
    parser();
    create_map();
}

void Map::draw_map(sf::RenderWindow& window) {
    window.clear();
    for (const auto& it : map) {
        window.draw((*it).sprite);
    }
    window.display();
}