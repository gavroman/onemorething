#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

#include "Map.h"

int main()
{
    Map battle_field("../source/game_map/Mixed_map.tmx");
    sf::RenderWindow window(sf::VideoMode(1625, 643), "One More Thing");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        battle_field.draw_map(window);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}