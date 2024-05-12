#define _USE_MATH_DEFINES

#include <iostream> 
#include <cmath>
#include <chrono>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/Event.hpp"

const float SHAPE_RADIUS = 70.0f;
const float CIRCLE_RADIUS = 200.0f;

sf::Vector2f calculatePositionOnCircle(float radius, float originX, float originY, long elapsedTimeMs, float revolutionPeriodMs);

int main()
{

    sf::RenderWindow window;

    window.create(sf::VideoMode(1500.0f, 800.0f), "Billiards");

    sf::CircleShape shape(SHAPE_RADIUS);
    shape.setFillColor(sf::Color::White);

    sf::Texture shapeTexture;
    shapeTexture.loadFromFile("assets/yellow_billiards_ball.png");
    shape.setTexture(&shapeTexture);

    sf::Event event;

    const auto startTimeMs = std::chrono::high_resolution_clock::now();

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        const auto currentTimeMs = std::chrono::high_resolution_clock::now();
        const auto elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTimeMs - startTimeMs).count();

        sf::Vector2f currentShapePosition = 
            calculatePositionOnCircle(CIRCLE_RADIUS, window.getSize().x / 2.0f, window.getSize().y / 2.0f, elapsedTimeMs, 4000.0f);
        shape.setPosition(currentShapePosition.x - SHAPE_RADIUS, currentShapePosition.y - SHAPE_RADIUS);

        window.clear(sf::Color(21, 88, 67, 255.0f));
        window.draw(shape);
        window.display();
    }

    return 0;
}


sf::Vector2f calculatePositionOnCircle(float radius, float originX, float originY, long elapsedTimeMs, float revolutionPeriodMs)
{
    const float currentAngle = 2 * M_PI * (elapsedTimeMs / revolutionPeriodMs);

    const float x = radius * cos(currentAngle) + originX;
    const float y = radius * sin(currentAngle) + originY;

    return sf::Vector2f(x, y);
}