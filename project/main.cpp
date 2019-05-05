#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

#include "Map.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1625, 643), "One More Thing");
    
    Map battle_field("../source/game_map/Dark_map.tmx");
    battle_field.draw_map(window);

    while (window.isOpen()) {
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