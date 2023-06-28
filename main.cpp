#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "quatree.hpp"
#include <windows.h>
#include <vector>
#include "point.hpp"
#include <chrono>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define POINTS 100000

#define RADIUS 200

void clearConsole()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My window");
    std::vector<std::shared_ptr<Point>> points = {};
    Quadtree quadtree(sf::Vector2f(0, 0), sf::Vector2f(window.getSize().x, window.getSize().y), 4);
    bool mousePressed = false;

    sf::RectangleShape rectangle = sf::RectangleShape(sf::Vector2f(100, 100));
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Green);
    rectangle.setOutlineThickness(1);
    rectangle.setOrigin(sf::Vector2f(50, 50));
    sf::Vector2f zoom = sf::Vector2f(1, 1);

    for (int i = 0; i < POINTS; i++) {
        points.push_back(std::make_shared<Point>(Point(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT)));
    }

    quadtree.rebuild(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT), points);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            
            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                quadtree.rebuild(sf::Vector2f(event.size.width, event.size.height), points);
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                mousePressed = true;
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                mousePressed = false;
            }

            if (event.type == sf::Event::MouseWheelScrolled) {
                zoom = sf::Vector2f(zoom.x + event.mouseWheelScroll.delta * -0.1, zoom.y + event.mouseWheelScroll.delta * -0.1);

                if (zoom.x < 0.1) {
                    zoom = sf::Vector2f(0.1, 0.1);
                }

                window.setView(sf::View(sf::FloatRect(0, 0, window.getSize().x * zoom.x, window.getSize().y * zoom.y)));
            }

            if (event.type == sf::Event::MouseMoved) 
            {
                rectangle.setPosition(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
            }

            if (mousePressed) {
                sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
                if (quadtree.insert(std::make_shared<Point>(mouse_position.x, mouse_position.y)))
                {
                    points.push_back(std::make_shared<Point>(mouse_position.x, mouse_position.y));
                }
                quadtree.insert(std::make_shared<Point>(mouse_position.x, mouse_position.y));
            }

            if (event.type == sf::Event::Closed)
                window.close();
        }

        
        
        // quadtree.rebuild(sf::Vector2f(window.getSize().x, window.getSize().y), points);
        window.clear();
        quadtree.drawGizmo(window);
        
        int checked = 0;
        std::vector<std::shared_ptr<Point>> points_in_range = quadtree.get_points_in_area(rectangle.getGlobalBounds(), checked);
        printf("Checked in rect: %d\n", checked);


        for (auto point : points_in_range) {
                sf::CircleShape circle = sf::CircleShape(5);
                circle.setFillColor(sf::Color::Red);
                circle.setPosition(point->get_position());
                window.draw(circle);
            }
        
        window.draw(rectangle);
        window.display();
    }
}