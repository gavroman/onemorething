#include <SFML/Graphics.hpp>
#include <unistd.h>
#include "Map.h"

int main()
{
    Map battle_field("../source/game_map/megamap_test.tmx");
    sf::RenderWindow window(sf::VideoMode(1250, 850), "One More Thing");
    battle_field.draw_map(window);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //window.clear();
        //window.display();
        sleep(0.01);
    }

    return 0;
}