#pragma once

#include <memory>
#include <vector>
#include <array>
#include <point.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class Quadtree
{
private:
    const int max_objects;
    sf::Vector2f position;
    sf::Vector2f size;

    int depth = 0;

    std::vector<std::shared_ptr<Point>> points = {};
    std::vector<std::shared_ptr<Quadtree>> children = {};
public:
    Quadtree(const sf::Vector2f &position, const sf::Vector2f &size, const int &max_objects, const int &depth = 0);
    ~Quadtree();

    void divide();
    void rebuild(const sf::Vector2f &size, std::vector<std::shared_ptr<Point>> &points);
    bool insert(const std::shared_ptr<Point> &point);
    bool insertInChildren(const std::shared_ptr<Point> &point);

    bool contains(const std::shared_ptr<Point> &point);
    bool contains(const sf::Rect<float> &rect);

    std::vector<std::shared_ptr<Point>> get_points_in_area(const sf::Rect<float> &rect, int &checked) const;

    void drawGizmo(sf::RenderWindow &window);

    friend std::ostream& operator<<(std::ostream& os, const Quadtree& obj)
    {
        os << "Quadtree(" << obj.position.x << ", " << obj.position.y << ", " << obj.size.x << ", " << obj.size.y << ")\n";
        os << "\t" << obj.points.size() << " points";
        
        return os;
    }
};
