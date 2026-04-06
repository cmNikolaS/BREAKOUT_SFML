#pragma once
#include <SFML/Audio.hpp>
extern sf::SoundBuffer soundBuffer;
extern sf::Sound ballSound;

constexpr float blockTopMargin = 50.f;
constexpr float blockScreenGap = 9.f;

constexpr float gravity = 150.f;
constexpr float ballMaxVelocityY = 700.f;
constexpr float bounceSpeed = 700.f;
constexpr float paddleMoveSpeed = 600.f;

constexpr unsigned int windowWidth = 600;
constexpr unsigned int windowHeight = 800;
constexpr float screenW = static_cast<float>(windowWidth);
constexpr float screenH = static_cast<float>(windowHeight);

constexpr float blockSizeX = 37.f;
constexpr float blockSizeY = 20.f;

constexpr int numberOfRows = 8;
constexpr int numberOfColumns = 14;

constexpr float spacing = 4.5f;

enum class GameState {
	Running,
	Menu
};