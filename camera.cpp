#include <cmath>
#include "camera.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include "MapObject.hpp"

Camera::Camera(float radius, float _speed = 0.f, float _x = 100, float _y = 100, float scale_ = 1.f) : x(_x), y(_y), speed(_speed ), scale(scale_)
{
    sf::CircleShape temp(radius * scale);
    cs = temp;
    cs.setFillColor(sf::Color::Green);
    cs.setPosition(x * scale, y * scale);
    cs.setOrigin(cs.getRadius(), cs.getRadius());

    speed *= scale;

    dir = sf::Vector2f(-1, 0);
    plane = sf::Vector2f(0, 0.66f);

    rayDepth = 75.0f * scale; 

}
void Camera::draw(sf::RenderWindow &window)
{
    const float SCREEN_WIDTH = window.getSize().x;
    const float SCREEN_HEIGHT = window.getSize().y;
    sf::VertexArray line(sf::TriangleFan, SCREEN_WIDTH + 1);
    line[0].position = cs.getPosition();
    line[0].color = sf::Color::Blue;

    sf::Vector2f endRay;
    sf::Vector2f prevPoint;

    for (size_t i = 0; i < SCREEN_WIDTH; ++i)
    {
        //get ray direction in vector 
        sf::Vector2f beginRay(x, y);
        sf::Vector2f rayDir;
        sf::Vector2f lenRay;
        float perpWallDist;



        float cameraX = 2 * i / static_cast<float>(SCREEN_WIDTH) - 1; //x-coordinate in camera space

        rayDir.x = dir.x + plane.x * cameraX;
        rayDir.y = dir.y + plane.y * cameraX;

        sf::Vector2i mapCheck;
        mapCheck.x = static_cast<int>(beginRay.x);
        mapCheck.y = static_cast<int>(beginRay.y);

        //DDA algorithm 
        sf::Vector2f cofficient(std::abs(1.0f / rayDir.x), std::abs(1.0f / rayDir.y));
        sf::Vector2i step;
        int side;

        if (rayDir.x < 0)
        {
            step.x = -1;
            lenRay.x = (beginRay.x - static_cast<float>(mapCheck.x)) * cofficient.x;
        }
        else 
        {
            step.x = 1;
            lenRay.x = (static_cast<float>(mapCheck.x + 1) - beginRay.x) * cofficient.x;
        }
        if (rayDir.y < 0)
        {
            step.y = -1;
            lenRay.y = (beginRay.y - static_cast<float>(mapCheck.y)) * cofficient.y;
        }
        else 
        {
            step.y = 1;
            lenRay.y = (static_cast<float>(mapCheck.y + 1) - beginRay.y) * cofficient.y;
        }

        float denominator, numerator_a, numerator_b, Ua, Ub;

        float currentDepth = 0.0f;
        bool isIntersec = false;

        while (!isIntersec && currentDepth < rayDepth)
        {
            if (lenRay.x < lenRay.y)
            {
                mapCheck.x += step.x;
                currentDepth = lenRay.x;
                lenRay.x += cofficient.x;
                side = 0;
            }
            else  
            {
                mapCheck.y += step.y;
                currentDepth = lenRay.y;
                lenRay.y += cofficient.y;
                side = 1;
            }

            endRay.x = beginRay.x + lenRay.x * rayDir.x;
            endRay.y = beginRay.y + lenRay.y * rayDir.y;

            for (size_t j = 1; j < MapObject::pointsOfObjectsMap.size(); j += 2)
            {
                sf::Vector2f a = MapObject::pointsOfObjectsMap[j - 1];
                sf::Vector2f b = MapObject::pointsOfObjectsMap[j];

                  denominator = (b.y - a.y) * (beginRay.x - endRay.x) - (b.x - a.x) * (beginRay.y - endRay.y);

                  if (denominator == 0)
                  {
                      if ((beginRay.x * endRay.y - endRay.x * beginRay.y) * (b.x - a.x) - (a.x * b.y - b.x * a.y) * (endRay.x - beginRay.x) == (beginRay.x * endRay.y - endRay.x * beginRay.y))
                      {
                          isIntersec = true;
                      }
                  }
                  else  
                  {
                      numerator_a = (b.x - endRay.x) * (b.y - a.y) - (b.x - a.x) * (b.y - endRay.y);
                      numerator_b = (beginRay.x - endRay.x) * (b.y - endRay.y) - (b.x - endRay.x) * (beginRay.y - endRay.y);

                      Ua = numerator_a / denominator;
                      Ub = numerator_b / denominator;

                      if (Ua >= 0 && Ua <= 1 && Ub >=0 && Ub <= 1)
                      {
                          isIntersec = true;
                      }
                  }
            }
        }

        if (side == 0) perpWallDist = lenRay.x - cofficient.x;
        else perpWallDist = lenRay.y - cofficient.y;

        if (i > 0 && i < SCREEN_WIDTH - 1)
        {
            beginRay = prevPoint;
        }

        line[i + 1].position = endRay;
        line[i + 1].color = sf::Color::Blue;
        prevPoint = endRay;

        float stripeHeight = std::round(SCREEN_HEIGHT / perpWallDist);


//      sf::RectangleShape earth(sf::Vector2f(1.f, SCREEN_HEIGHT - stripeHeight));
//      earth.setPosition(static_cast<float>(SCREEN_WIDTH - 1 - i), SCREEN_HEIGHT - stripeHeight);
//      earth.setFillColor(sf::Color(153, 76, 0));

        sf::RectangleShape stripe(sf::Vector2f(1.f, stripeHeight));
        stripe.setPosition(static_cast<float>(SCREEN_WIDTH - 1- i), 0.5f * (SCREEN_HEIGHT - stripeHeight));
        int shade = 255 * (1 - currentDepth / rayDepth);
//      unsigned char brightness = std::round(std::max<float>(0, 255 * (2 * currentDepth / camera.rayDepth - 1)));
//      stripe.setFillColor(sf::Color(64, 80, 222, brightness));
        stripe.setFillColor(sf::Color(shade, shade, shade));

//      window.draw(earth);
        window.draw(stripe);
    }
}
void Camera::turnRight(float const& times)
{
    float time = times / 10;
    float oldDirX = dir.x;
    dir.x = dir.x * cosf(speed * time) - dir.y * sinf(speed * time);
    dir.y = oldDirX * sinf(speed * time) + dir.y * cosf(speed * time);

    float oldPlaneX = plane.x;
    plane.x = plane.x * cosf(speed * time) - plane.y * sinf(speed * time);
    plane.y = oldPlaneX * sinf(speed * time) + plane.y * cosf(speed * time);

    cs.setPosition(x, y);
}
void Camera::turnLeft(float const& times)
{
    float time = times / 10;
    float oldDirX = dir.x;
    dir.x = dir.x * cosf(-speed * time) - dir.y * sinf(-speed * time);
    dir.y = oldDirX * sinf(-speed * time) + dir.y * cosf(-speed * time);

    float oldPlaneX = plane.x;
    plane.x = plane.x * cosf(-speed * time) - plane.y * sinf(-speed * time);
    plane.y = oldPlaneX * sinf(-speed * time) + plane.y * cosf(-speed * time);
    cs.setPosition(x, y);
}
void Camera::moveForward(float const& time)
{
    x += dir.x * speed * time; 
    y += dir.y * speed * time; 
    cs.setPosition(x, y);
}
void Camera::moveBack(float const& time)
{    
    x -= dir.x * speed * time; 
    y -= dir.y * speed * time; 
    cs.setPosition(x, y);
}