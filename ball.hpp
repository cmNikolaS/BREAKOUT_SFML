#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include "defaults.hpp"
#include "entity.hpp"

class Ball : public Entity{
private:
	sf::Vector2f defaultPos;
	sf::Vector2f velocity = { 0.f, gravity };
	sf::Vector2f ogVelocity;

public:
	Ball(std::shared_ptr<sf::Texture> text, const sf::Vector2f &pos,const sf::Vector2f &targetSize) : Entity(text, pos, targetSize)
	{
		defaultPos = pos;
		ogVelocity = velocity;
	}

	const sf::Vector2f &getVelocity() const
	{
		return velocity;
	}
	const float getVelocityX() const
	{
		return velocity.x;
	}
	const float getVelocityY() const
	{
		return velocity.y;
	}

	void update(const float dt)
	{
		velocity.y += gravity * dt;
		auto lastPos = position;
		move({ velocity.x * dt, velocity.y * dt });
		if (position.x >= screenW - size.x -1 || position.x <= 1)
		{
			velocity.x = -velocity.x;
			setPos(lastPos);
			ballSound.play();
		}
		if (position.y <= 0)
		{
			setPos(lastPos);
			velocity.y = -velocity.y;
			ballSound.play();
		}
		if (position.y >= screenH)
		{
			restart();
		}
	}
	void setVelocityX(float vel)
	{
		velocity.x = vel;
	}
	void setVelocityY(float vel)
	{
		velocity.y = vel;
	}
	void decreaseVelocityY()
	{
		velocity.y = -bounceSpeed;
	}
	void restart()
	{
		velocity = ogVelocity;
		position = defaultPos;
		sprite.setPosition(defaultPos);
	}
};