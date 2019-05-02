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
    int* neighbours; // Массив указателей на айдишники соседних гексов
    sf::Sprite sprite; // Спрайт с текстурой
    int x; // Координаты центра
    int y; //
};

class Map {
 public:
    // Map(); Заготовка под пустой конструктор для процедурной генерации карты
    Map(std::string xml_file_path);
    void parser(); // Заполняет поля на строках 34-39
    std::unique_ptr<Cell> create_hex(); // Возвращает указатель на заполненную текстурку
    void create_map(); // Заполняет вектор смежности

    // TODO Отрисовка карты

 protected:
    std::vector <std::unique_ptr<Cell>> map; // Вектор смежности карты

 private:
    std::ofstream xml_file; //  Файловый дескриптор
    unsigned int map_size_width; // Ширина карты
    unsigned int map_size_height; // Высота карты
    unsigned int hex_size_width; // Ширина гекса
    unsigned int hex_size_height; // Высота гекса
    sf::Texture map_texture; // Текстура карты
};

#endif //ONEMORETHING_MAP_H
