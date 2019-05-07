#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

#include "Map.h"

int main(const int argc, const char ** argv) {
    //sf::RenderWindow window(sf::VideoMode(1920, 1080), "One More Thing", sf::Style::Fullscreen);
    sf::RenderWindow window(sf::VideoMode(1920, 750), "One More Thing");

    std::vector<std::string> maps_names;                    //
    maps_names.emplace_back("Dark_map.tmx");                //
    maps_names.emplace_back("Grass_map.tmx");               // Исключительно для тестов.
    maps_names.emplace_back("Mixed_map.tmx");               //
    maps_names.emplace_back("Mixed_map_v2.tmx");            // TODO(): Убрать это все в класс игры
    maps_names.emplace_back("Snow_map.tmx");                //
    int map_number = (argc == 2) ? std::atoi(argv[1]) : 0;  //

    Map battle_field("../source/game_map/" + maps_names[map_number]);

    sf::Vector2i pos_pressed(0, 0);
    sf::Vector2i pos_released(0, 0);
    battle_field.draw_map(window);
    while (window.isOpen()) {
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
                    if (pos_pressed == pos_released and pos_pressed.x and pos_pressed.y) {
                        sf::Vector2f pos(pos_pressed.x, pos_pressed.y);
                        int cell_id = battle_field.get_cell_id_from_pos(pos);
                        battle_field.draw_map(window);
                        window.display();
                        window.draw(battle_field.highlight_cell(cell_id, sf::Color(20, 30, 52, 100), sf::Color::Cyan));
                        window.display();
                        std::cout << "Pressed cell id = " << cell_id <<std::endl;

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