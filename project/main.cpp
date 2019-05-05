#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <iostream>

#include "Map.h"



int main(const int argc, const char ** argv) {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "One More Thing", sf::Style::Fullscreen);

    std::vector<std::string> maps_names;                    // 
    maps_names.emplace_back("Dark_map.tmx");                // 
    maps_names.emplace_back("Grass_map.tmx");               // Исключительно для тестов.
    maps_names.emplace_back("Mixed_map.tmx");               //  
    maps_names.emplace_back("Mixed_map_v2.tmx");            // TODO(): Убрать это все куда-то
    maps_names.emplace_back("Snow_map.tmx");                // 
    int map_number = (argc == 2) ? std::atoi(argv[1]) : 0;  // 

    Map battle_field("../source/game_map/" + maps_names[map_number]);

    while (window.isOpen()) {
        battle_field.draw_map(window);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}