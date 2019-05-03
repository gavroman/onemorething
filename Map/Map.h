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

#include "./../Character/Character.h"

struct Cell {
    std::unique_ptr <Character> character; // Указатель на объект типа "Персонаж"
    std::vector<int> neighbours; // Массив указателей на айдишники соседних гексов
    sf::Sprite sprite; // Спрайт с текстурой
    int x; // Координаты центра
    int y; //
    bool passability; // Проходимость
};

class Map {
 public:
    // Map(); Заготовка под пустой конструктор для процедурной генерации карты
    explicit Map(const std::string& xml_file_path);
    void parser(); // Заполняет поля класса
    std::shared_ptr<Cell> create_hex(); // Возвращает указатель на заполненную текстурку
    void create_map(); // Заполняет вектор смежности

    std::vector<int> search_neighbours(const int id);

    void draw_map(sf::RenderWindow& window); // Отрисовывает карту

    void proceed_click();
    void update_cell();

 protected:
    std::vector<std::shared_ptr<Cell>> map; // Вектор смежности карты

 private:
    std::ifstream xml_file; //  Файловый дескриптор
    unsigned int map_size_width; // Ширина карты
    unsigned int map_size_height; // Высота карты
    unsigned int hex_size_width; // Ширина гекса
    unsigned int hex_size_height; // Высота гекса
    sf::Texture map_texture; // Текстура карты, загружается из одной картинки и содержит в себе все тайлы (так же, как это организовано в tiled map editor)
};

#endif //ONEMORETHING_MAP_H
