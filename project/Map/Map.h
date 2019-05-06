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
    std::unique_ptr<Character> character; // Указатель на объект типа "Персонаж"
    std::vector<int> neighbors;           // Массив указателей на айдишники соседних гексов
    sf::Sprite sprite;                    // Спрайт с текстурой
    float x;                              // Координаты левого
    float y;                              //  верхнего угла
    bool passability;                     // Проходимость
};

class Map {
 public:
    // Map(); Заготовка под пустой конструктор для процедурной генерации карты
    explicit Map(std::string xml_file);
    std::shared_ptr<Cell> create_hex();                      // Возвращает указатель на заполненную текстурку
    std::vector<int> search_neighbors(const int id);
    sf::Vector2f calculate_position(const int id);           // Вычисляет смещения для отрисовки
    void draw_map(sf::RenderWindow& window);                 // Отрисовывает карту
    int get_cell_id_from_pos(const sf::Vector2f & pos);
    sf::Vector2f get_cell_center(const int id);
    float calculate_distance(sf::Vector2f point1, sf::Vector2f point2);
    void update_cell();

 protected:
    std::vector<std::shared_ptr<Cell>> map; // Вектор смежности карты

 private:
    std::string xml_file_path;  // Путь к файлу
    int map_size_width;         // Ширина карты
    int map_size_height;        // Высота карты
    
    int hex_size_width;         // Ширина гекса
    int hex_size_height;        // Высота гекса
    float hex_offset_y = 15;

    float scale = 1.15;         // map scale
    float offset_x = 20;
    float offset_y = 180;

    sf::Texture map_texture;    // Текстура карты
};

#endif //ONEMORETHING_MAP_H
