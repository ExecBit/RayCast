#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

struct Camera 
{
    float x;
    float y;

    sf::Vector2f dir;
    sf::Vector2f plane;

    float rayDepth;
    float speed;
    float scale;

    sf::CircleShape cs;

    Camera(float radius, float _speed, float _x, float _y, float scale_);

    void draw(sf::RenderWindow &window);

    void turnLeft(float const& times);
    void turnRight(float const& times);
    void moveBack(float const& time);
    void moveForward(float const& time);

};