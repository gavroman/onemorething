//
// Created by arugaf on 02.05.19.
//

#include "Map.h"

Map::Map(std::string xml_file_path) {
    parser();
    create_map();
}