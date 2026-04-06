#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
class Entity {
protected:
	sf::Vector2f position;
	std::shared_ptr<sf::Texture> texture;
	sf::Sprite sprite;
	sf::Vector2f size;

public:

	Entity(const std::shared_ptr<sf::Texture> text, const sf::Vector2f& pos, const sf::Vector2f& size)
		: texture(text), position(pos), size(size), sprite(*text)
	{
		setSize(size);
		sprite.setPosition(pos);
	}

	void setPos(const sf::Vector2f& pos)
	{
		sprite.setPosition(pos);
		position = pos;
	}
	const sf::Vector2f& getPos() const
	{
		return position;
	}
	void move(const sf::Vector2f& move)
	{
		sprite.move(move);
		position += move;
	}
	void setSize(const sf::Vector2f& newSize)
	{
		size = newSize;
		sprite.setScale({ newSize.x / texture->getSize().x, newSize.y / texture->getSize().y });
	}
	const sf::Vector2f getSize() const
	{
		return size;
	}
	const sf::FloatRect getGlobalBounds() const
	{
		return sprite.getGlobalBounds();
	}
	void draw(sf::RenderWindow& window) const
	{
		window.draw(sprite);
	}
	void setSizeX(const float newSizeX)
	{
		setSize({ newSizeX, size.y });
	}
	void setSizeY(const float newSizeY)
	{
		setSize({ size.x, newSizeY });
	}
	const float getSizeX() const
	{
		return size.x;
	}
	const float getSizeY() const
	{
		return size.y;
	}
	
	void updateTexture(std::shared_ptr<sf::Texture> newTexture)
	{
		const sf::Vector2f& oldSize = size;
		texture = newTexture;
		sprite.setTexture(*texture);
		setSize(oldSize);
	}

	const auto getTexture() const
	{
		return texture;
	}

	const sf::Sprite& getSprite() const
	{
		return sprite;
	}
};