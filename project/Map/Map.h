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
    std::vector<int> neighbors; // Массив указателей на айдишники соседних гексов
    sf::Sprite sprite; // Спрайт с текстурой
    int x; // Координаты центра
    int y; //
    bool passability; // Проходимость
};

class Map {
 public:
    // Map(); Заготовка под пустой конструктор для процедурной генерации карты
    explicit Map(std::string xml_file);
    std::shared_ptr<Cell> create_hex(); // Возвращает указатель на заполненную текстурку

    std::vector<int> search_neighbors(const int id);
    
    sf::Vector2f calculate_position(const int id); // Вычисляет смещения для отрисовки
    void draw_map(sf::RenderWindow& window); // Отрисовывает карту

    void proceed_click(const sf::Vector2i pos);
    void update_cell();

 protected:
    std::vector<std::shared_ptr<Cell>> map; // Вектор смежности карты

 private:
    std::string xml_file_path;  // Путь к файлу
    int map_size_width;         // Ширина карты
    int map_size_height;        // Высота карты
    int hex_size_width;         // Ширина гекса
    int hex_size_height;        // Высота гекса

    float scale = 1.;     //  TODO(): когда появится класс персонажа, 
    float offset_x = 0;//20;    //  эти параметры надо будет перенести
    float offset_y = 0;   //  в какой-то главный/общий хедер

    sf::Texture map_texture; // Текстура карты, загружается из одной картинки и содержит в себе все тайлы (так же, как это организовано в tiled map editor)
};

#endif //ONEMORETHING_MAP_H
