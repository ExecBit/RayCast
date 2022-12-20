#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

struct MapObject
{
    private:
    sf::RectangleShape shape;

    public:
    static std::vector<sf::Vector2f> pointsOfObjectsMap;
    explicit MapObject(sf::Vector2f size, const sf::Color &color, float x, float y, float scale_);
    void refreshPoint();
    sf::RectangleShape& getObject();
    static void clearPointOfObjects(std::vector<sf::Vector2f> &points);
    float scale;
};
