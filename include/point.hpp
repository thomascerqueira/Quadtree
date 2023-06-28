#pragma once
#include <SFML/System.hpp>

class Point
{
private:
    sf::Vector2f position;
public:
    Point(const float &x, const float &y);
    Point(const sf::Vector2f &position);

    sf::Vector2f get_position() const { return position; }
    ~Point();
};


