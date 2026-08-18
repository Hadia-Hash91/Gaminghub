#include "Knife.h"

Knife::Knife()
{
	texture.loadFromFile("IMAGES/knife_strip.png");
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, frameW, frameH));
	sprite.setOrigin(frameW / 2.f, static_cast<float>(frameH)); // pivot from bottom-center
}

void Knife::fire(sf::Vector2f startPos)
{
	active = true;
	currentFrame = 0;
	animTimer = 0.f;
	sprite.setPosition(startPos);
}

void Knife::update(float dt)
{
	if (!active) return;

	sprite.move(0.f, -speed * dt); // move upward

	animTimer += dt;
	if (animTimer >= frameDuration)
	{
		animTimer = 0.f;
		currentFrame = (currentFrame + 1) % frameCount; // loop while flying
		sprite.setTextureRect(sf::IntRect(currentFrame * frameW, 0, frameW, frameH));
	}

	if (sprite.getPosition().y < -frameH) // fully off top of screen
		active = false;
}

void Knife::draw(sf::RenderWindow& window)
{
	if (active)
		window.draw(sprite);
}

sf::FloatRect Knife::getBounds() const
{
	sf::FloatRect b = sprite.getGlobalBounds();
	b.top += boundsTopTrim;
	b.height -= (boundsTopTrim + boundsBottomTrim);
	b.left += boundsSideTrim;
	b.width -= (boundsSideTrim * 2.f);
	return b;
}