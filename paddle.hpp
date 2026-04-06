#pragma once
#include <SFML\Graphics.hpp>
#include "defaults.hpp"
#include "entity.hpp"

class Paddle : public Entity{
private:
    sf::Vector2f velocity = { 0, 0 }, ogPosition = { 0, 0};
public:
    Paddle(std::shared_ptr<sf::Texture> text, const sf::Vector2f &position, const sf::Vector2f &size) : Entity(text, position, size), ogPosition(position)
    {
    }
    sf::Vector2f getVelocity()
    {
        return velocity;
    }
    void handleInput()
    {
        velocity.x = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            velocity.x = -paddleMoveSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            velocity.x = paddleMoveSpeed;

        }
    }
    void update(float dt)
    {
        auto lastPos = position;
        move({ velocity.x * dt, 0 });
        if (position.x >= screenW - getSizeX() || position.x <= 0)
            setPos(lastPos);

    }
    void restart()
    {
        position = ogPosition;
        sprite.setPosition(ogPosition);
        velocity.x = 0;
    }
};