#include <SFML/Graphics.hpp>
#include <unistd.h>

#include "

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "One More Thing");


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(hex);
        window.display();
        sleep(0.05);
    }

    return 0;
}