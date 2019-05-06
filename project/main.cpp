#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

#include "Map.h"

int main(const int argc, const char ** argv) {
    sf::RenderWindow window(sf::VideoMode(1029, 800), "One More Thing");//, sf::Style::Fullscreen);

    std::vector<std::string> maps_names;                    //
    maps_names.emplace_back("Dark_map.tmx");                //
    maps_names.emplace_back("Grass_map.tmx");               // Исключительно для тестов.
    maps_names.emplace_back("Mixed_map.tmx");               //
    maps_names.emplace_back("Mixed_map_v2.tmx");            // TODO(): Убрать это все куда-то
    maps_names.emplace_back("Snow_map.tmx");                //
    int map_number = (argc == 2) ? std::atoi(argv[1]) : 0;  //

    Map battle_field("../source/game_map/" + maps_names[map_number]);

    sf::Vector2i pos_pressed(0, 0);
    sf::Vector2i pos_released(0, 0);
    while (window.isOpen()) {
        battle_field.draw_map(window);
        sf::Event event;
        while (window.pollEvent(event)) {
            switch(event.type) {   
                case sf::Event::Closed:
                    window.close(); 
                    break;
    
                case sf::Event::MouseButtonPressed: 
                    pos_pressed = sf::Mouse::getPosition(window);
                    break;
    
                case sf::Event::MouseButtonReleased:
                    pos_released = sf::Mouse::getPosition(window);

                    if (pos_pressed == pos_released) {
                        sf::Vector2f pos(pos_pressed.x, pos_pressed.y);
                        battle_field.proceed_click(pos);
                    }
                    break;
                
                default: 
                    break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}