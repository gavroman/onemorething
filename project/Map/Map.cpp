#include "Map.h"
#include "tinyxml2.h"
Map::Map(const std::string& xml_file_path) 
    : xml_file(xml_file_path) {
    parse();
}

void Map::parse() {
    using namespace tinyxml2;
    XMLDocument doc;
    if (doc.LoadFile("../source/game_map/megamap.tmx")) {
        return;
    }
    XMLElement * map_xml = doc.FirstChildElement("map");
    if (map_xml == nullptr) {;
        return;
    }
    map_size_width = map_xml->IntAttribute("width", 0);
    map_size_height = map_xml->IntAttribute("height", 0);
    tile_size_width = map_xml->IntAttribute("tilewidth", 0);
    tile_size_height = map_xml->IntAttribute("tileheight", 0);  

    std::vector<std::string> tilesets_srcs;
    XMLElement * tileset_xml = map_xml->FirstChildElement("tileset");
    do {
        std::string src = tileset_xml->Attribute("source");
        tilesets_srcs.emplace_back(src.erase(0,2));
    } while ((tileset_xml = tileset_xml->NextSiblingElement("tileset")));
    for (auto& it: tilesets_srcs) {
        std::cout << it << std::endl;        
    }

    sf::Image hex_image;
    hex_image.loadFromFile("../source/game_map/" + tilesets_srcs[1]); //создаем объект Image (изображение)
    sf::Texture hex_texture;//создаем объект Texture (текстура)
    hex_texture.loadFromImage(hex_image);//передаем в него объект Image (изображения)
    sf::Sprite hex_sprite;//создаем объект Sprite(спрайт)
    hex_sprite.setTexture(hex_texture);//передаём в него объект Texture (текстуры)
    hex_sprite.setPosition(50, 25);//задаем начальные координаты появления спрайта*/
    //std::vector<XMLElement *> layers;
    //XMLElement * layer = map_xml->FirstChildElement("layer");
    //if(layer) {
    //  layers.emplace_back(layer);
    //}
    XMLElement * data_xml = map_xml->FirstChildElement("layer")->FirstChildElement("data");
    XMLElement * tile_xml = nullptr;
    int i = 0;
    for (tile_xml = data_xml->FirstChildElement("tile"); tile_xml; tile_xml = tile_xml->NextSiblingElement("tile")) {
        //std::cout << i << std::endl;

        std::shared_ptr cell = std::make_shared<Cell>();
        cell->id = i++;
        cell->character = nullptr;
        //cell->sprite = 
        map.emplace_back(cell);

    }
}

void Map::draw_map(sf::RenderWindow& window) {
    window.clear();
    for (const auto& it : map) {
        window.draw((*it).sprite);
    }
    window.display();
}