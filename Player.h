#pragma once
#include "Knife.h"

#include <SFML/Graphics.hpp>


class Player
{
private:
	sf::Texture idleTexture, shootTexture,knifetexture;
	sf::Sprite sprite;

	int currentFrame = 0;
	float animTimer = 0.f;
	float frameDuration = 0.12f;
	bool facingLeft = false;
	// frame cell sizes (from the strips)
	const int idleFrameW = 866 / 7;   // 7 frames now
	const int idleFrameH = 288;
	const int idleFrameCount = 7;

	const int shootFrameW =824/8 ;
	const int shootFrameH = 303;
	const int shootFrameCount = 8;
	Knife knife;

	int lives = 3;
	bool isHit = false;          // NEW: brief invincibility flag
	float hitCooldown = 0.f;

	float shootFrameDuration = 0.12f;  // faster playback for the shoot animation
	bool isShooting = false;
	bool knifeJustFired = false;
	sf::RectangleShape player2;

	float playerSpeed = 60.f; // Speed of the player movement
	float dt = 0.016f; // Assuming a fixed time step of 60 FPS (1/60 seconds)

	sf::Clock clock;

public:
	Player();
	void handleInput();
	void update();
	void draw(sf::RenderWindow& window);
	void animate();

	sf::FloatRect getKnifeBounds() const { return knife.getBounds(); }
	bool isKnifeActive() const { return knife.isActive(); }

	int getLives() const { return lives; }
	void loseLife();
	sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
};