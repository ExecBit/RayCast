#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <cmath>
#include <cstddef>
#include <iostream>
#include <pthread.h>
#include <vector>
#include "MapObject.hpp"
#include "camera.hpp"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const float SCALE = 1.f;

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "MyProgramm");
    window.setFramerateLimit(60);

    Camera camera(4.0f, 0.05f, 40, 40, SCALE);

    MapObject obj1(sf::Vector2f(80.f, 80.f), sf::Color::Red, 70, 70, SCALE);
    MapObject obj2(sf::Vector2f(90.f, 90.f), sf::Color::Red, 300, 300, SCALE);
    MapObject obj3(sf::Vector2f(90.f, 90.f), sf::Color::Red, 70, 250, SCALE);
    MapObject obj4(sf::Vector2f(90.f, 90.f), sf::Color::Red, 350, 70, SCALE);
    MapObject border(sf::Vector2f(120.f, 120.f), sf::Color::Black, 5, 5, SCALE * 5);

    border.getObject().setOutlineThickness(3.f);
    border.getObject().setOutlineColor(sf::Color::Red);
    border.refreshPoint();


    sf::RectangleShape sky(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    sky.setFillColor(sf::Color(0, 255, 255));


    float time{0};
    sf::Clock clock;
    while (window.isOpen())
    {
        time = clock.restart().asMicroseconds();
        time /= 1500;

        std::string title = "Engine " + std::to_string(1000.f / time)  + " fps"; 
        window.setTitle(title);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            camera.turnLeft(time);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            camera.turnRight(time);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            camera.moveForward(time);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
        {
            camera.moveBack(time);
        }


        window.clear();
        camera.draw(window);

//      window.draw(sky);
//      window.draw(border.getObject());
//      window.draw(line);

//      window.draw(camera.cs);

//      window.draw(obj1.getObject());
//      window.draw(obj2.getObject());
//      window.draw(obj3.getObject());
//      window.draw(obj4.getObject());
        window.display();
    }
    return 0;
}