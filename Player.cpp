#include"Player.h"


Player::Player()
{
	idleTexture.loadFromFile("IMAGES/idle_strip.png");
	shootTexture.loadFromFile("IMAGES/shoot_strip.png");

	sprite.setTexture(idleTexture);
	sprite.setTextureRect(sf::IntRect(0, 0, idleFrameW, idleFrameH));
	sprite.setPosition(375.f, 600.f);

	

	player2.setSize(sf::Vector2f(50.f, 50.f)); // Set the size of the playerd
	player2.setFillColor(sf::Color::Black); // Set the color of the player
	player2.setPosition(500.f, 520.f);
	// Set the initial position of the player
}

void Player::handleInput()
{
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		sprite.move(-playerSpeed * dt, 0.f); // Move left
		facingLeft = true;
	}
	   
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		sprite.move(playerSpeed * dt, 0.f); // Move right
		facingLeft = false;

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isShooting)
	{
		isShooting = true;
		currentFrame = 0;
		sprite.setTexture(shootTexture);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		player2.move(-playerSpeed * dt, 0.f); // Move left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		player2.move(playerSpeed * dt, 0.f); // Move right
}

void Player::update()
{
	dt = clock.restart().asSeconds();
	handleInput();
	animate();
	knife.update(dt);

	if (isHit)   // NEW
	{
		hitCooldown -= dt;
		if (hitCooldown <= 0.f)
			isHit = false;
	}
}

void Player::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	window.draw(player2);
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
			sprite.setTexture(idleTexture);
			sprite.setOrigin(idleFrameW / 2.f, idleFrameH);   // reset origin for idle
		}
		else
		{
			sprite.setTextureRect(sf::IntRect(currentFrame * shootFrameW, 0, shootFrameW, shootFrameH));
			sprite.setOrigin(shootFrameW / 2.f, shootFrameH); // bottom-center of shoot frame
		}
	}
	else
	{
		currentFrame = (currentFrame + 1) % idleFrameCount;
		sprite.setTextureRect(sf::IntRect(currentFrame * idleFrameW, 0, idleFrameW, idleFrameH));
		sprite.setOrigin(idleFrameW / 2.f, idleFrameH);       // bottom-center of idle frame
	}
}


