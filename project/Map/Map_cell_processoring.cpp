#include <iostream>
#include <cmath>

#include "Map.h"
#include "Character.h"
#include "Player.h"

/* В этом файле содержится все, что касается обработки кликов по клеткам
 * В том числе отрисовка состояния клетки (кликнута, наведен курсор, куда можно пойти и т.п.)*/
bool Map::compare_positions(const sf::Vector2f& pos1, const sf::Vector2f& pos2) {
    float diff = calculate_distance(pos1, pos2);
    if (diff < 1000) { // TODO: протестить с мышкой
        return true;
    }
    return false;
}

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

void Map::add_highlight_cells(const std::vector<int> ids, sf::Color color, sf::Color border_color) {
    for (auto& it : ids) {
        std::shared_ptr hghlt = std::make_shared<Highlighting>();
        hghlt->id = it;
        hghlt->fill_color = color; 
        hghlt->border_color = border_color;
        highlighted_cells.emplace_back(hghlt);
    }
}

void Map::drop_highlight_cells() {
    highlighted_cells = {};
}

float Map::calculate_distance(sf::Vector2f p1, sf::Vector2f p2) { //Norma in this space
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

bool Map::is_empty(const int id) {
    if (id >= 0) {
        return map[id]->character == nullptr;
    }
    return true;
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

void Map::get_adj_matrix() {
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
    adj_matrix = matrix;
}

std::vector<std::vector<int>> Map::find_move_area(const int id, const int distance) {
    std::vector<std::vector<int>> trace;
    trace.push_back({id});
    for (int i = 1; i < distance + 1; i++) {
        std::vector<int> trace_distance;
        for (int j = 0; j < trace[i - 1].size(); j++) {
            int k = 0;

            while (adj_matrix[k][0] != trace[i - 1][j]) {
                k++;
            }

            std::vector<int> neighbors = adj_matrix[k];
            trace_distance.insert(std::end(trace_distance), std::begin(neighbors), std::end(neighbors));
        }
        sort(trace_distance.begin(), trace_distance.end());
        std::vector<int>::iterator it = unique(trace_distance.begin(), trace_distance.end());
        trace_distance.resize(it - trace_distance.begin());
        trace.push_back(trace_distance);
    }
    return trace;
}

std::vector<int> Map::find_route(const int id, const std::vector<std::vector<int>> trace) {
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
        while (adj_matrix[k][0] != id_add) {
            k++;
        }
        std::vector<int> neighbors = adj_matrix[k];
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

sf::Vector2f Map::get_cell_pos(const int id) {
    return sf::Vector2f(map[id]->sprite.getPosition().x, map[id]->sprite.getPosition().y);
}

void Map::update_cell(std::shared_ptr<Character> pers, int id) {
    int old_id = pers->get_current_cell();
    map[id]->character = pers;
    map[id]->character->update_id(id);
    if (id != old_id) {
        map[old_id]->character = nullptr;
    }
}

std::vector<sf::Vector2f> Map::discrete_positions(const int id1, const int id2, const int step) {
    std::vector<sf::Vector2f> positions;
    std::cout << "cell id = " << id1 << "    "<< get_cell_pos(id1).x << "   "<< get_cell_pos(id1).y <<std::endl;
    float dx = (get_cell_pos(id2).x - get_cell_pos(id1).x) / step;
    float dy = (get_cell_pos(id2).y - get_cell_pos(id1).y) / step;
    for (int i = 0; i != step; i++) {
        float pos_x = get_cell_pos(id1).x + dx * i;    
        float pos_y = get_cell_pos(id1).y + dy * i;
        positions.emplace_back(sf::Vector2f(pos_x, pos_y));
    }
    return positions;
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
