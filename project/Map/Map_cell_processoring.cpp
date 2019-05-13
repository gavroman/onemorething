#include "Map.h"
#include "Character.h"
#include "Player.h"
#include <iostream>

/* В этом файле содержится все, что касается обработки кликов по клеткам
 * В том числе отрисовка состояния клетки (кликнута, наведен курсор, куда можно пойти и т.п.)*/

int Map::get_cell_id_from_pos(const sf::Vector2f & pos) {
    std::vector<int> candidates_id;
    for (auto& cell : map) {
        sf::FloatRect coords = cell->sprite.getGlobalBounds();
        if (coords.contains(pos.x, pos.y)) {
            candidates_id.emplace_back(cell->id);
        }
        if (candidates_id.size() == 2) {
            break;
        }
    }  
    if (candidates_id.size() == 2) {                         // collision proceed
        std::vector<sf::Vector2f> candidates_center = {
            get_cell_center(map[candidates_id[0]]->id),
            get_cell_center(map[candidates_id[1]]->id)
        };
        float dist1 = calculate_distance(pos, candidates_center[0]);
        float dist2 = calculate_distance(pos, candidates_center[1]);

        if (dist1 < dist2) {
            return candidates_id[0];
        } else {
            return candidates_id[1];
        }
    } else if(candidates_id.size() == 1){
        return candidates_id[0];
    }
    return -1;
}

sf::Vector2f Map::get_cell_center(const int id) {
    sf::Vector2f center(map[id]->x + hex_size_width * scale / 2, map[id]->y + hex_size_height  * scale / 2);
    return center;
}

sf::CircleShape Map::highlight_cell(const int id, sf::Color color, sf::Color border_color) {
    sf::CircleShape hex_shape(71, 6);
    hex_shape.setPosition(map[id]->x - 4.6, map[id]->y - 0.3);
    hex_shape.setScale(scale, scale);
    hex_shape.setFillColor(color);
    hex_shape.setOutlineThickness(2);
    hex_shape.setOutlineColor(border_color);
    return hex_shape;
}

float Map::calculate_distance(sf::Vector2f p1, sf::Vector2f p2) { //Norma in this space
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

bool Map::is_passable(const int id) {
    if (id >= 0) {
        return map[id]->passability;
    }
    return false;
}

bool Map::is_in_area(const std::vector<std::vector<int>> area, const int id) {
    return std::find(area[area.size() - 1].begin(), 
                     area[area.size() - 1].end(),
                     id) != area[area.size() - 1].end();
}

std::vector<std::vector<int>> Map::get_adj_matrix() {
    std::vector<std::vector<int>> matrix;
    for (int id = 0; id < map_size_width * map_size_height; id++) {
        if (map[id]->passability) {
            std::vector<int> neighbors = search_neighbors(id);
            for (int i = 0; i < neighbors.size(); i++) {
                if (!map[neighbors[i]]->passability) {
                    neighbors.erase(neighbors.begin() + i);
                    i--;
                }
            }
            neighbors.insert(neighbors.begin(), id);
            matrix.push_back(neighbors);
        }
    }
    /*for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout<<std::endl;
    }*/
    return matrix;
}

std::vector<std::vector<int>> Map::find_move_area(const int id, const std::vector<std::vector<int>> matrix_adj, const int distance) {
    std::vector<std::vector<int>> trace;
    trace.push_back({id});
    for (int i = 1; i < distance + 1; i++) {
        std::vector<int> trace_distance;
        for (int j = 0; j < trace[i - 1].size(); j++) {
            int k = 0;
            while (matrix_adj[k][0] != trace[i - 1][j]) {
                k++;
            }
            std::vector<int> neighbors = matrix_adj[k];
            trace_distance.insert(std::end(trace_distance), std::begin(neighbors), std::end(neighbors));
        }
        sort(trace_distance.begin(), trace_distance.end());
        std::vector<int>::iterator it = unique(trace_distance.begin(), trace_distance.end());
        trace_distance.resize(it - trace_distance.begin());
        trace.push_back(trace_distance);
    }
    return trace;
}

std::vector<int> Map::find_route(const int id, const std::vector<std::vector<int>> trace, const std::vector<std::vector<int>> matrix_adj) {
    std::vector<int> one_trace;
    int id_add = id;
    one_trace.push_back(id_add);
    int i = 1;
    for (i; i < trace.size(); i++) {
        if (std::find(trace[i].begin(), trace[i].end(), id) != trace[i].end()) {
            break;
        }
    }
    for (i; id_add != trace[0][0]; i--) {
        int k = 0;
        while (matrix_adj[k][0] != id_add) {
            k++;
        }
        std::vector<int> neighbors = matrix_adj[k];
        for (int j = 0; j < trace[i - 1].size(); j++) {
            auto find_id = std::find(neighbors.begin(), neighbors.end(), trace[i - 1][j]);
            if (find_id != neighbors.end()) {
                id_add = find_id[0];
                one_trace.push_back(id_add);
                break;
            }
        }
    }
    return one_trace;
}    

void Map::update_cell(std::shared_ptr<Character> character, int id) {
    /*if (!map[id]->character) {

        map[id]->character = character;
    } else {
        map[id]->character = nullptr;
    }
*/
    int cell_id = map[id]->character->get_current_cell();
    map[id]->character = character;
    map[cell_id]->character = nullptr;

}

/*
void Map::proceed_click(const int& id) {
    if () { //   Проверка на нахождение в области видимости
        if (map[id]->character) { // Проверяем на интерактивность
            if ((players[current_player]->is_my_char(map[id]->character))) { // Проверяем владельца
                if (!(map[id]->character->is_active())) { // Активируем персонажа
                    map[id]->character->set_active();
                    return;
                }
            } else { // если это чужой персонаж
                if (players[current_player]->get_active_char()) {
                    players[current_player]->get_active_char()->apply_damage(map[id]);
                    return;
                }
            }
        }
    } else {
        if (players[current_player]->get_active_char()) {
            players[current_player]->move_character(get_route(players[current_player]->get_active_char()->get_current_cell(), id), players[current_player]->get_active_char());
        }
    }
}
*/
