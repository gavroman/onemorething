#include "Map.h"
#include "tinyxml2.h"

Map::Map(std::string xml_file)
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
    std::vector<sf::Sprite> hex_sprites;
    int current_x = 0;
    int current_y = 0;
    for (int i = 0; i != sprite_count; i++) {
        if (!(i % sprite_columns) && i) {
            current_y += sprite_height;
            current_x = 0;
        }
        hex_sprites.emplace_back(sf::Sprite(map_texture, sf::Rect(current_x, current_y, sprite_width, sprite_height)));
        current_x += sprite_width;
    }

    XMLElement * tile_xml = map_xml->FirstChildElement("layer")->FirstChildElement("data")->FirstChildElement("tile");

    int i = 0;
    do {
        std::shared_ptr cell = std::make_shared<Cell>();
        cell->id = i++;
        cell->character = nullptr;
        cell->neighbors = search_neighbors(cell->id);
        int gid = tile_xml->IntAttribute("gid", 0);

        cell->sprite = hex_sprites[gid - 1];
        cell->passability = (gid <= 12); // Первые 12 тайлов проходимы
        cell->sprite.setPosition(calculate_position(cell->id));
        map.emplace_back(cell);
    } while ((tile_xml = tile_xml->NextSiblingElement("tile")));

    //===============вывод id и id соседей===============
    /*for (auto& cell : map) {
        std::cout << "id: " << cell->id << " neighbors: ";
        for (auto& neighbor : cell->neighbors) {
            std::cout << neighbor << ", ";
        }
        std::cout << std::endl;
    }*/
    //===================================================
}

sf::Vector2f Map::calculate_position(const int id) {
    int row = id / map_size_width;
    int col = id % map_size_width;
    float pos_x = col * hex_size_width;
    float pos_y = row * (hex_size_height - 15) ;
    if (row % 2 == 0) {
        pos_x += (float)hex_size_width / 2;
    }
    sf::Vector2f position(pos_x, pos_y);
    return position;
}

std::vector<int> Map::search_neighbors(const int id) { // TODO(9rik): fixme
    std::vector<int> sosed{
            id - 2 * map_size_width,
            id - map_size_width,
            id - map_size_width + 1,
            id + map_size_width - 1,
            id + map_size_width,
            id + 2 * map_size_width
    };
    std::vector<int> neighbors;

    if (id == 0) {
        neighbors.push_back(sosed[5]);
        neighbors.push_back(sosed[4]);
        return neighbors;
    }

    if (((map_size_height % 2 == 0) and (id == map_size_width * map_size_height - 1)) or ((map_size_height % 2 != 0) and (id == map_size_width * (map_size_height - 1)))) {
        neighbors.push_back(sosed[0]);
        neighbors.push_back(sosed[1]);
        return neighbors;
    }

    if (id < map_size_width) {
        for (int j = 3; j < sosed.size(); j++) {
            neighbors.push_back(sosed[j]);
        }
        return neighbors;
    }

    if (id >= map_size_width * (map_size_height - 1)) {
        for (int j = 0; j < 3; j++) {
            neighbors.push_back(sosed[j]);
        }
        if (map_size_height % 2 != 0) {
            neighbors[2] -= 2;
        }
        return neighbors;
    }

    if (((id + 1) % (2 * map_size_width) == 0) or (id % (2 * map_size_width) == 0)) {
        if (sosed[0] >= 0) {
            neighbors.push_back(sosed[0]);
        }
        neighbors.push_back(sosed[1]);
        neighbors.push_back(sosed[4]);
        if (sosed[5] < map_size_width * map_size_height) {
            neighbors.push_back(sosed[5]);
        }
        return neighbors;
    }

    if (((id - (id % map_size_width)) % (2 * map_size_width)) == 0) {
        for (int j = 0; j < sosed.size(); j++) {
            if (j == 2) {
                neighbors.push_back(sosed[j] - 2);
                continue;
            }
            if ((sosed[j] >= 0) and (sosed[j] < map_size_width * map_size_height)) {
                neighbors.push_back(sosed[j]);
            }
        }
        return neighbors;
    }

    for (int j = 0; j < sosed.size(); j++) {
        if (j == 3) {
            neighbors.push_back(sosed[j] + 2);
            continue;
        }
        if ((sosed[j] >= 0) and (sosed[j] < map_size_width * map_size_height)) {
            neighbors.push_back(sosed[j]);
        }
    }
    return neighbors;
}

void Map::draw_map(sf::RenderWindow& window) {
    window.clear();
    for (const auto& it : map) {
        window.draw((*it).sprite);
    }
    window.display();
}