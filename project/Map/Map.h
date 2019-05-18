//
// Created by arugaf on 28.04.19.
//

#ifndef ONEMORETHING_MAP_H
#define ONEMORETHING_MAP_H

#include <sys/param.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <string>
#include <fstream>
#include <iostream>

#include "Character.h"
#include "Player.h"

struct Cell {
    int id;
    std::shared_ptr<Character> character; // Указатель на объект типа "Персонаж"
    std::vector<int> neighbors;           // Массив указателей на айдишники соседних гексов
    sf::Sprite sprite;                    // Спрайт с текстурой
    float x;                              // Координаты левого
    float y;                              //  верхнего угла
    bool passability;                     // Проходимость
};

struct Highlighting {
    int id;
    sf::Color fill_color;
    sf::Color border_color;
};

class Map {
 public:
    // Map(); Заготовка под пустой конструктор для процедурной генерации карты
    explicit Map(const std:: string xml_file);

    void draw_map(sf::RenderWindow& window);                 // Отрисовывает карту
    void add_highlight_cells(const std::vector<int> ids, sf::Color color, sf::Color border_color);
    void drop_highlight_cells();
    sf::CircleShape highlight_cell(const int id, sf::Color color, sf::Color border_color);

    void proceed_click(const int& id);

    int get_cell_id_from_pos(const sf::Vector2f & pos);      // Достает id по координатам
    bool compare_positions(const sf::Vector2f& pos1, const sf::Vector2f& pos2);
    std::vector<std::vector<int>> get_adj_matrix();          // Матрица смежности
    std::vector<std::vector<int>> find_move_area(const int id, 
                                                 const std::vector<std::vector<int>> matrix_adj, 
                                                 const int distance); //возможные пути с учетом дистанции

    std::vector<int> find_route(const int id, 
                                const std::vector<std::vector<int>> trace, 
                                const std::vector<std::vector<int>> matrix_adj);
    bool is_empty(const int id);
    bool is_passable(const int id);
    bool is_in_area(const std::vector<std::vector<int>> area, const int id);

    void set_active_char(const int id, bool active);
    void update_cell(std::shared_ptr<Character> character, int id);


    std::vector <int> get_route(int start, int end); 
    // Должен срабатывать: 1) когда выбран персонаж И кликнута клетка, куда пойдет персонаж 
    //                     2) когда выбран персонаж и наведен курсор на клетку в пределах 
    //                        рэйнджа этого персонажа
    sf::Vector2f get_cell_pos(const int id);
    std::vector<sf::Vector2f> discrete_positions(const int id1, const int id2, const int step);

 private:
    std::vector<int> search_neighbors(const int id);         // Заполняет вектор соседей
    sf::Vector2f calculate_position(const int id);           // Вычисляет смещения для отрисовки

    sf::Vector2f get_cell_center(const int id);              // Вычисляет центр клетки
    static float calculate_distance(sf::Vector2f point1, sf::Vector2f point2);

    std::vector<std::shared_ptr<Cell>> map; // Вектор смежности карты
    std::vector<std::shared_ptr<Highlighting>> highlighted_cells;
    std::string xml_file_path;  // Путь к файлу
    int map_size_width;         // Ширина карты
    int map_size_height;        // Высота карты
    
    int hex_size_width;         // Ширина гекса
    int hex_size_height;        // Высота гекса
    float hex_offset_y = 35;

    float scale = 0.465;         // map scale
    float offset_x = 30;
    float offset_y = 400;

    sf::Texture map_texture;    // Текстура карты
};

#endif //ONEMORETHING_MAP_H
