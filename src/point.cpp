#include "../include/point.hpp"

Point::Point(const float &x, const float &y): position(x, y)
{
}

Point::Point(const sf::Vector2f &position): position(position)
{
}

Point::~Point()
{
}