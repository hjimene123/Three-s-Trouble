#include <SFML/Graphics.hpp>
#include <iostream>

class Player {
public:
    sf::RectangleShape shape;
    float velocity;

    Player(float x, float y) {
        shape.setSize(sf::Vector2f(30, 30));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Green);
        velocity = 0;
    }

    void flap() {
        velocity = -400.0f;
    }

    void update(float deltaTime) {
        velocity += 0.8f;
        shape.move(0, velocity * deltaTime);

       
        if (shape.getPosition().y < 0) {
            shape.setPosition(shape.getPosition().x, 0);
            velocity = 0;
        }
        else if (shape.getPosition().y + shape.getSize().y > 600) {
            std::cout << "Game Over! Hit the bottom of the screen." << std::endl;
            exit(EXIT_SUCCESS);
        }
    }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }
};

class BouncingSquare {
public:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    sf::Clock timer;

    BouncingSquare(float x, float y, float initialVelocityX, float initialVelocityY, sf::Color color) {
        shape.setSize(sf::Vector2f(30, 30));
        shape.setPosition(x, y);
        shape.setFillColor(color);
        velocity = sf::Vector2f(initialVelocityX, initialVelocityY);
        timer.restart();
    }

    void update(float deltaTime) {
        shape.move(velocity * deltaTime);

        
        if (shape.getPosition().x < 0 || shape.getPosition().x + shape.getSize().x > 800) {
            velocity.x = -velocity.x;
        }

       
        if (shape.getPosition().y < 0 || shape.getPosition().y + shape.getSize().y > 600) {
            velocity.y = -velocity.y;
        }

       
        if (timer.getElapsedTime().asSeconds() > 30.0f) {
            velocity *= 1.2f; 
            timer.restart();
        }
    }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Three's Trouble");

    
    float playerInitialX = (window.getSize().x - 30) / 2.0f;
    float playerInitialY = (window.getSize().y - 30) / 2.0f;

    Player player(playerInitialX, playerInitialY);

    
    const int numSquares = 3;
    std::vector<BouncingSquare> squares;

    squares.push_back(BouncingSquare(700, 200, -200.0f, 150.0f, sf::Color::Red));
    squares.push_back(BouncingSquare(500, 400, 150.0f, -100.0f, sf::Color::Blue));
    squares.push_back(BouncingSquare(200, 300, 100.0f, 200.0f, sf::Color::Yellow));

    sf::Clock clock;
    sf::Clock totalTimeClock;
    float deltaTime = 0.0f;

    float highScore = 0.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
                player.flap();
        }

        deltaTime = clock.restart().asSeconds();

        player.update(deltaTime);

        for (auto& square : squares) {
            square.update(deltaTime);

            if (player.getBounds().intersects(square.getBounds())) {
                std::cout << "Game Over! A square got you." << std::endl;

                
                float currentSurvivalTime = totalTimeClock.getElapsedTime().asSeconds();
                if (currentSurvivalTime > highScore) {
                    highScore = currentSurvivalTime;
                    std::cout << "New High Score: " << highScore << " seconds!" << std::endl;
                }
                else {
                    std::cout << "Survived for: " << currentSurvivalTime << " seconds." << std::endl;
                }

                exit(EXIT_SUCCESS);
            }

            window.clear();

            window.draw(player.shape);

            for (const auto& square : squares) {
                window.draw(square.shape);
            }

            window.display();
        }
    }

    return 0;
}
