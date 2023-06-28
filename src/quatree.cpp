#include "../include/quatree.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

Quadtree::Quadtree(const sf::Vector2f &position, const sf::Vector2f &size, const int &max_objects, const int &depth): max_objects(max_objects), position(position), size(size), depth(depth)
{
    points.reserve(max_objects);
}

Quadtree::~Quadtree()
{
}


void Quadtree::divide()
{
    sf::Vector2f next_size = sf::Vector2f(size.x / 2, size.y / 2);
    std::array<sf::Vector2f, 4> positions = {
        position,
        sf::Vector2f(position.x + next_size.x, position.y),
         sf::Vector2f(position.x, position.y + next_size.y),
         sf::Vector2f(position.x + next_size.x, position.y + next_size.y)
    };

    for (auto _position: positions) {
        children.push_back(std::make_shared<Quadtree>(_position, next_size, max_objects, depth + 1));
    }

    for (auto child: children) {
        for (auto point: points) {
            if (child->contains(point)) {
                child->insert(point);
            }
        }
    }
}

void Quadtree::rebuild(const sf::Vector2f &size, std::vector<std::shared_ptr<Point>> &points)
{
    this->size = size;
    children.clear();

    for (auto point: points) {
        insert(point);
    }
}

std::vector<std::shared_ptr<Point>> Quadtree::get_points_in_area(const sf::Rect<float> &rect, int &checked) const
{
    std::vector<std::shared_ptr<Point>> points_in_area = {};

    for (auto point: points) {
        checked++;
        if (rect.contains(point->get_position())) {
            points_in_area.push_back(point);
        }
    }

    for (auto child: children) {
        if (child->contains(rect))
        {
            std::vector<std::shared_ptr<Point>> child_points = child->get_points_in_area(rect, checked);
            points_in_area.insert(points_in_area.end(), child_points.begin(), child_points.end());
        }
    }

    return points_in_area;
}


bool Quadtree::contains(const sf::Rect<float> &rect)
{
    sf::Rect<float> rect2 = sf::Rect<float>(position, size);

    return rect.intersects(rect2);
}

bool Quadtree::contains(const std::shared_ptr<Point> &point)
{
    sf::Vector2f point_position = point->get_position();
    return point_position.x >= position.x && point_position.x <= position.x + size.x && point_position.y >= position.y && point_position.y <= position.y + size.y;
}

bool Quadtree::insertInChildren(const std::shared_ptr<Point> &point)
{
    for (auto child: children) {
        if (child->contains(point)) {
            return child->insert(point);
        }
    }

    return false;
}

bool Quadtree::insert(const std::shared_ptr<Point> &point)
{
    if (points.size() < max_objects)
    {
        points.push_back(point);
        return true;
    }
    else
    {
        if (children.size() == 0)
        {
            divide();
            return insertInChildren(point);
        }
        else
        {
            return insertInChildren(point);
        }
    }
    
    return false;
}

void Quadtree::drawGizmo(sf::RenderWindow &window)
{
    sf::RectangleShape rectangle;
    rectangle.setSize(size);
    rectangle.setPosition(position);
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color(255, 255, 255, 50));
    rectangle.setOutlineThickness(1);
    window.draw(rectangle);

    if (children.size() != 0)
    {
        for (auto child: children) {
            child->drawGizmo(window);
        }
    } else {
        for (auto point: points) {
            sf::CircleShape circle;
            circle.setRadius(1);
            circle.setPosition(point->get_position());
            circle.setFillColor(sf::Color((depth * 30) %255, 0, 0, 255));
            window.draw(circle);
        }
    }
}