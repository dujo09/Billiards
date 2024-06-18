#include <iostream>
#include <SFML/Graphics.hpp>
#include "solver.hpp"
#include "renderer.hpp"
#include "utils/number_generator.hpp"
#include "utils/math.hpp"


static sf::Color getRainbow(float t)
{
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * Math::PI);
    const float b = sin(t + 0.66f * 2.0f * Math::PI);
    return { static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b) };
}


int32_t main(int32_t, char* [])
{
    // Create window
    constexpr int32_t window_width = 500;
    constexpr int32_t window_height = 500;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Verlet", sf::Style::Default, settings);
    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);

    Solver   solver;
    Renderer renderer{ window };

    // Solver configuration
    solver.setConstraint({ static_cast<float>(window_width), static_cast<float>(window_height) });
    solver.setSubStepsCount(8);
    solver.setSimulationUpdateRate(frame_rate);

    // Set simulation attributes
    const float        object_spawn_delay = 0.025f;//0.025f
    const float        object_spawn_speed = 1200.0f;//1200.0f
    const sf::Vector2f object_spawn_position = { 250.0f, 250.0f };
    const float        object_min_radius = 10.0f;//10.0f
    const float        object_max_radius = 30.0f;//30.0f
    const uint32_t     max_objects_count = 10;//1000
    const float        max_angle = 1.0f;

    sf::Clock clock;
    sf::Vector2i lastMousePosition = { -1,-1 };
    // Main loop
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                if (lastMousePosition.x < 0) {
                    lastMousePosition = mousePosition;
                }
                else {
                    sf::Vector2i delta = mousePosition - lastMousePosition;
                    solver.setAllObjectsVelocity({ (float)delta.x*2, (float)delta.y*2 });
                    lastMousePosition = { -1,-1 };
                }


            }
        }

        if (solver.getObjectsCount() < max_objects_count && clock.getElapsedTime().asSeconds() >= object_spawn_delay) {
            clock.restart();
            auto& object = solver.addObject(object_spawn_position, RNGf::getRange(object_min_radius, object_max_radius));
            const float t = solver.getTime();
            const float angle = max_angle * sin(t) + Math::PI * 0.5f;
            solver.setObjectVelocity(object, object_spawn_speed * sf::Vector2f{ cos(angle), sin(angle) });
            object.color = getRainbow(t);
        }

        solver.update();
        window.clear(sf::Color::White);
        renderer.render(solver);
        window.display();
    }

    return 0;
}


//#define _USE_MATH_DEFINES
//
//#include <iostream> 
//#include <cmath>
//#include <chrono>
//
//#include "SFML/Graphics/RenderWindow.hpp"
//#include "SFML/Graphics/CircleShape.hpp"
//#include "SFML/Graphics/Texture.hpp"
//#include "SFML/Window/Event.hpp"
//
//const float SHAPE_RADIUS = 70.0f;
//const float CIRCLE_RADIUS = 200.0f;
//
//sf::Vector2f calculatePositionOnCircle(float radius, float originX, float originY, long elapsedTimeMs, float revolutionPeriodMs);
//
//int main()
//{
//
//    sf::RenderWindow window;
//
//    window.create(sf::VideoMode(1500.0f, 800.0f), "Billiards");
//
//    sf::CircleShape shape(SHAPE_RADIUS);
//    shape.setFillColor(sf::Color::White);
//
//    sf::Texture shapeTexture;
//    shapeTexture.loadFromFile("assets/yellow_billiards_ball.png");
//    shape.setTexture(&shapeTexture);
//
//    sf::Event event;
//
//    const auto startTimeMs = std::chrono::high_resolution_clock::now();
//
//    while (window.isOpen())
//    {
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        const auto currentTimeMs = std::chrono::high_resolution_clock::now();
//        const auto elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTimeMs - startTimeMs).count();
//
//        sf::Vector2f currentShapePosition = 
//            calculatePositionOnCircle(CIRCLE_RADIUS, window.getSize().x / 2.0f, window.getSize().y / 2.0f, elapsedTimeMs, 4000.0f);
//        shape.setPosition(currentShapePosition.x - SHAPE_RADIUS, currentShapePosition.y - SHAPE_RADIUS);
//
//        window.clear(sf::Color(21, 88, 67, 255.0f));
//        window.draw(shape);
//        window.display();
//    }
//
//    return 0;
//}
//
//
//sf::Vector2f calculatePositionOnCircle(float radius, float originX, float originY, long elapsedTimeMs, float revolutionPeriodMs)
//{
//    const float currentAngle = 2 * M_PI * (elapsedTimeMs / revolutionPeriodMs);
//
//    const float x = radius * cos(currentAngle) + originX;
//    const float y = radius * sin(currentAngle) + originY;
//
//    return sf::Vector2f(x, y);
//}
//
