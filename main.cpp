#include <SFML/Graphics.hpp>
#include <unistd.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "One More Thing");
    sf::CircleShape hex(100, 6);
    hex.setFillColor(sf::Color::Green);

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
        sleep(1);
    }

    return 0;
}