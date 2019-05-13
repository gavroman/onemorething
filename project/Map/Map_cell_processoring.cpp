#include "Map.h"
#include "Character.h"
#include "Player.h"

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

float Map::calculate_distance(sf::Vector2f p1, sf::Vector2f p2) { //Norma in this space
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

sf::Vector2f Map::get_cell_center(const int id) {
    sf::Vector2f center(map[id]->x + hex_size_width * scale / 2, map[id]->y + hex_size_height  * scale / 2);
    return center;
}

sf::CircleShape Map::highlight_cell(const int id, sf::Color color, sf::Color border_color)  {
    sf::CircleShape hex_shape(72, 6);
    hex_shape.setPosition(map[id]->x - 4, map[id]->y);
    hex_shape.setScale(scale, scale);
    hex_shape.setFillColor(color);
    hex_shape.setOutlineThickness(2);
    hex_shape.setOutlineColor(border_color);
    return hex_shape;
}

/*
void Map::proceed_click(const int& id) {
    if (Проверка на нахождение в области видимости) {
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
