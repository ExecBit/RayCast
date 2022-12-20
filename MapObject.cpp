#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <iostream>
#include <vector>
#include "MapObject.hpp"

std::vector<sf::Vector2f> MapObject::pointsOfObjectsMap;

void MapObject::refreshPoint() 
{
    sf::Transform matrix = shape.getTransform();

    std::vector<sf::Vector2f> pointOfObject;
    for (size_t i = 0; i < shape.getPointCount(); ++i)
    {
        sf::Vector2f point = matrix.transformPoint(shape.getPoint(i));
        pointOfObject.push_back(point);
    }
    
    MapObject::pointsOfObjectsMap.push_back(pointOfObject[0]);
    size_t currentIndex = 1;
    for (size_t i = 1; i < shape.getPointCount() * 2 - 1; ++i)
    {
        MapObject::pointsOfObjectsMap.push_back(pointOfObject[currentIndex]);
        if (i % 2 == 0) ++currentIndex;

    }
    MapObject::pointsOfObjectsMap.push_back(pointOfObject[0]);
}

MapObject::MapObject(sf::Vector2f size, const sf::Color &color, float x, float y, float scale_ = 1.f) : scale(scale_)
{
  shape = sf::RectangleShape(size * scale);
  shape.setFillColor(color);
  shape.setPosition(x * scale, y * scale);

  this->refreshPoint();
}
sf::RectangleShape& MapObject::getObject()
{
    return shape;
}

void MapObject::clearPointOfObjects(std::vector<sf::Vector2f> &points)
{
    points.clear();
}
