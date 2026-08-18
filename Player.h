#pragma once
#include "Knife.h"

#include<SFML/Graphics.hpp>


class Player
{
private:
	sf::Sprite sprite;

	int currentFrame = 0;
	float animTimer = 0.f;
	float frameDuration = 0.10f;
	bool facingLeft = false;

	sf::Texture idleTextures[6];
	sf::Texture shootTextures[6];
	int activeCharacter = 0; // which one is currently playing
	// frame cell sizes (from the strips)
	const int idleFrameW =  707/ 6;   // 7 frames now
	const int idleFrameH = 353;
	const int idleFrameCount = 6;

	const int shootFrameW = 707 / 7;
	const int shootFrameH = 353;
	const int shootFrameCount = 7;
	Knife knife;

	int lives = 5;
	bool isHit = false;          // brief invincibility flag
	float hitCooldown = 0.f;

	float shootFrameDuration = 0.12f;  // playback for the shoot animation
	bool isShooting = false;

	float playerSpeed = 220.f; // Speed of the player movement
	float dt = 0.016f;

	
	static const float kCharSpeed[6];
	static const int kCharLives[6];

	sf::Clock clock;

	// How much empty padding to trim off the collision box, in pixels.
	// The idle/shoot frames are sized to fit the tallest pose (raised arm etc),
	// so there's usually a chunk of transparent space above the character's head.
	// Tune these two numbers while watching the hitbox overlay (H key) - nudge
	// topTrim up if the box still starts above the head, down if it cuts into it.
	float boundsTopTrim = 90.f;
	float boundsBottomTrim = 10.f;

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
	void resetLives() { lives = kCharLives[activeCharacter]; isHit = false; hitCooldown = 0.f; }
	void resetWeapon() { knife.deactivate(); isShooting = false; currentFrame = 0; animTimer = 0.f; }

	sf::FloatRect getBounds() const
	{
		sf::FloatRect b = sprite.getGlobalBounds();
		b.top += boundsTopTrim;
		b.height -= (boundsTopTrim + boundsBottomTrim);
		return b;
	}

	void setCharacter(int index);
};