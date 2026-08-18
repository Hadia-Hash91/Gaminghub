#include "Player.h"

const float Player::kCharSpeed[6] = { 220.f, 260.f, 200.f, 280.f, 180.f, 240.f };
const int Player::kCharLives[6] = { 3, 2, 4, 2, 5, 3 };

Player::Player()
{
	idleTextures[0].loadFromFile("IMAGES/idle_strip.png");
	idleTextures[1].loadFromFile("IMAGES/ch2_idle_strip.png");  //animation of players
	idleTextures[2].loadFromFile("IMAGES/ch3_idle_strip1.png");

	shootTextures[0].loadFromFile("IMAGES/shoot_strip.png");
	shootTextures[1].loadFromFile("IMAGES/ch2_shoot_strip.png");
	shootTextures[2].loadFromFile("IMAGES/ch3_shoot_strip.png");

	// Characters 4-6: load their strips if you add them later; until then
	// they reuse character 1's art so selecting them still works.
	const char* idleNames[6] = {
		"IMAGES/idle_strip.png", "IMAGES/ch2_idle_strip.png", "IMAGES/ch3_idle_strip.png",
		"IMAGES/ch4_idle_strip.png", "IMAGES/ch5_idle_strip.png", "IMAGES/ch6_idle_strip.png"
	};
	const char* shootNames[6] = {
		"IMAGES/shoot_strip.png", "IMAGES/ch2_shoot_strip.png", "IMAGES/ch3_shoot_strip.png",
		"IMAGES/ch4_shoot_strip.png", "IMAGES/ch5_shoot_strip.png", "IMAGES/ch6_shoot_strip.png"
	};
	for (int i = 0; i < 6; ++i) {
		if (!idleTextures[i].loadFromFile(idleNames[i])) idleTextures[i] = idleTextures[0];
		if (!shootTextures[i].loadFromFile(shootNames[i])) shootTextures[i] = shootTextures[0];
	}

	sprite.setTexture(idleTextures[0]);
	sprite.setTextureRect(sf::IntRect(0, 0, idleFrameW, idleFrameH));
	sprite.setOrigin(idleFrameW / 2.f, static_cast<float>(idleFrameH));
	sprite.setPosition(400.f, 560.f);
}

void Player::handleInput()
{
	float move = 0.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		move -= playerSpeed * dt;
		facingLeft = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		move += playerSpeed * dt;
		facingLeft = false;
	}

	sprite.move(move, 0.f);

	sf::Vector2f pos = sprite.getPosition();
	if (pos.x < 60.f) pos.x = 60.f;
	if (pos.x > 740.f) pos.x = 740.f;
	sprite.setPosition(pos);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isShooting)
	{
		isShooting = true;
		currentFrame = 0;
		animTimer = 0.f;
		sprite.setTexture(shootTextures[activeCharacter]);
	}
}

void Player::update()
{
	dt = clock.restart().asSeconds();
	if (dt > 0.05f) dt = 0.05f;   // clamp big frame spikes

	handleInput();
	animate();
	knife.update(dt);

	if (isHit)
	{
		hitCooldown -= dt;
		if (hitCooldown <= 0.f)
			isHit = false;
	}
}

void Player::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	knife.draw(window);
}

void Player::loseLife()
{
	if (isHit) return;   // already just got hit, ignore for now

	lives--;
	isHit = true;
	hitCooldown = 1.f;    // 1 second of "safe" time after getting hit
}

void Player::animate()
{
	sprite.setScale(facingLeft ? -1.f : 1.f, 1.f);
	animTimer += dt;
	if (animTimer < shootFrameDuration) return;
	animTimer = 0.f;

	if (isShooting)
	{
		currentFrame++;
		if (currentFrame == 5)
		{
			sf::Vector2f handPos = sprite.getPosition();
			handPos.y -= shootFrameH * 0.6f; // roughly near the raised hand, tweak this
			knife.fire(handPos);
		}

		if (currentFrame >= shootFrameCount)
		{
			currentFrame = 0;
			isShooting = false;
			sprite.setTexture(idleTextures[activeCharacter]);
			sprite.setOrigin(idleFrameW / 2.f, static_cast<float>(idleFrameH));
		}
		else
		{
			sprite.setTextureRect(sf::IntRect(currentFrame * shootFrameW, 0, shootFrameW, shootFrameH));
			sprite.setOrigin(shootFrameW / 2.f, static_cast<float>(shootFrameH)); // bottom-center of shoot frame
		}
	}
	else
	{
		currentFrame = (currentFrame + 1) % idleFrameCount;
		sprite.setTextureRect(sf::IntRect(currentFrame * idleFrameW, 0, idleFrameW, idleFrameH));
		sprite.setOrigin(idleFrameW / 2.f, static_cast<float>(idleFrameH));       // bottom-center of idle frame
	}
}

void Player::setCharacter(int index)
{
	if (index < 0 || index > 5) return;

	activeCharacter = index;
	playerSpeed = kCharSpeed[index];
	lives = kCharLives[index];
	currentFrame = 0;
	animTimer = 0.f;
	isShooting = false;
	facingLeft = false;
	sprite.setTexture(idleTextures[index]);
	sprite.setTextureRect(sf::IntRect(0, 0, idleFrameW, idleFrameH));
	sprite.setOrigin(idleFrameW / 2.f, static_cast<float>(idleFrameH));
	sprite.setScale(1.f, 1.f);
}
