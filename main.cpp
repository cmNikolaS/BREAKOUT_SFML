#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "paddle.hpp"
#include "ball.hpp"
#include "defaults.hpp"

void blocksInit(std::vector<Entity>& blocks, const std::unordered_map<std::string, std::shared_ptr<sf::Texture>>& textures);
void update(Ball& ball, Paddle& paddle, std::vector<Entity>& blocks, float dt);
void drawBlocks(sf::RenderWindow& window, const std::vector<Entity>& blocks);

int main()
{
    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "BREAKOUT");

	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
	
	auto loadTex = [&](const std::string& name, const std::string& file) {
		textures[name] = std::make_shared<sf::Texture>();
		textures[name]->loadFromFile(file);
		};

	loadTex("paddle", "paddle.png");
	loadTex("background", "background.png");
	loadTex("ball", "ball.png");
	loadTex("block_pink", "block_pink.png");
	loadTex("block_green", "block_green.png");
	loadTex("block_blue", "block_blue.png");
	loadTex("block_brown", "block_brown.png");

	loadTex("menu_background", "menu_background.png");
	Entity menu_background(textures["menu_background"], { 0, 0 }, { screenW, screenH });
	Entity background_gameRunning(textures["background"], {0,0}, { screenW, screenH });
	Paddle paddle(textures["paddle"], {250, 700}, {100, 20});
	Ball ball(textures["ball"], {290, 400}, {20, 20});
	std::vector<Entity> blocks;
	blocksInit(blocks, textures);

	sf::Clock clock;
    window.setFramerateLimit(165);

	GameState gameState = GameState::Menu;

	sf::Music gameMusic;
	gameMusic.openFromFile("djlofi-pixel-dreams-259187.mp3");
	gameMusic.setVolume(70);
	bool gameMusicRunning = true;
	gameMusic.play();
	gameMusic.setLooping(true);
		
	ballSound.setVolume(30);

	while (window.isOpen())
	{
        float dt = clock.restart().asSeconds();
		while (const std::optional event = window.pollEvent())
		{
            if (event->is<sf::Event::Closed>()) window.close();

			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->code == sf::Keyboard::Key::Q)
				{
					window.close();
				}
				if (keyPressed->code == sf::Keyboard::Key::M)
				{
					if (gameMusicRunning)
						gameMusic.pause();
					else
						gameMusic.play();
					gameMusicRunning = !gameMusicRunning;
				}
				if (keyPressed->code == sf::Keyboard::Key::Escape)
				{
					if (gameState == GameState::Running)
					{
						gameState = GameState::Menu;
					}
					else if (gameState == GameState::Menu)
					{
						gameState = GameState::Running;
						blocksInit(blocks, textures);
						ball.restart();
						paddle.restart();
					}
				}
			}
		}

		window.clear();

		if (gameState == GameState::Running)
		{
			paddle.handleInput();
			paddle.update(dt);

			ball.update(dt);

			update(ball, paddle, blocks, dt);

			if (blocks.empty()) gameState = GameState::Menu;

			background_gameRunning.draw(window);
			paddle.draw(window);
			ball.draw(window);
			drawBlocks(window, blocks);
		}
		else if (gameState == GameState::Menu)
		{
			menu_background.draw(window);
		}

        window.display();
	}
	return 0;
}

void drawBlocks(sf::RenderWindow& window, const std::vector<Entity>& blocks)
{
	for (const auto& block : blocks)
		block.draw(window);
}

void blocksInit(std::vector<Entity>& blocks, const std::unordered_map<std::string, std::shared_ptr<sf::Texture>>& textures)
{
	blocks.clear();
	blocks.reserve(numberOfRows * numberOfColumns);
	std::string c;
	for (int i = 0; i < numberOfRows; i++)
	{
		for (int j = 0; j < numberOfColumns; j++)
		{
			int r = i * numberOfColumns + j;
			if (r % 4 == 0)
			{
				c = "block_green";
			}
			else if (r % 4 == 1)
			{
				c = "block_brown";
			}
			else if (r % 4 == 3)
			{
				c = "block_blue";
			}
			else {
				c = "block_pink";
			}
			blocks.emplace_back(
				textures.at(c),
				sf::Vector2f{ blockScreenGap + (j + 1) * spacing + j * blockSizeX,  blockTopMargin + spacing * i + i * blockSizeY },
				sf::Vector2f{ blockSizeX, blockSizeY }
			);
		}
	}
}

void update(Ball& ball, Paddle& paddle, std::vector<Entity> &blocks, float dt)
{
	auto overlap = ball.getGlobalBounds().findIntersection(paddle.getGlobalBounds());
	
	if (overlap)
	{
		ballSound.play();

	float ballCenterX = ball.getPos().x + ball.getSize().x / 2.f;
	float paddleCenterX = paddle.getPos().x + paddle.getSizeX() / 2.f;

	float hitPos = ballCenterX - paddleCenterX;

	ball.setPos({
			ball.getPos().x,
			paddle.getPos().y - ball.getSize().y
		});


	ball.decreaseVelocityY();

	float normalized = hitPos / (paddle.getSizeX() / 2.f);

	normalized = std::clamp(normalized, -0.8f, 0.8f);

	ball.setVelocityX(normalized * bounceSpeed);

	return;
	}
	
	int pos = -1;

	for (int i = 0; i < blocks.size(); i++)
	{
		overlap = ball.getGlobalBounds().findIntersection(blocks[i].getGlobalBounds());
		if (overlap)
		{
			pos = i;
			break;
		}
	}
	if (pos == -1) return;
	
	auto& block = blocks.at(pos);

	float ballCX = ball.getPos().x + ball.getSize().x / 2.f;
	float ballCY = ball.getPos().y + ball.getSize().y / 2.f;
	float blockCX = block.getPos().x + block.getSize().x / 2.f;
	float blockCY = block.getPos().y + block.getSize().y / 2.f;

	if (std::abs(ballCX - blockCX) / block.getSize().x > std::abs(ballCY - blockCY) / block.getSize().y)
		ball.setVelocityX(-ball.getVelocityX());
	else
		ball.setVelocityY(-ball.getVelocityY());

	std::swap(blocks[pos], blocks.back());
	blocks.pop_back();
	ballSound.play();
}