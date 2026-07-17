#include "Knife.h"

Knife::Knife()
{
    texture.loadFromFile("IMAGES/knife_strip.png");
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, frameW, frameH));
    sprite.setOrigin(frameW / 2.f, frameH); // pivot from bottom-center
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
        currentFrame++;
        if (currentFrame >= frameCount)
        {
            active = false; // done animating, deactivate
            return;
        }
        sprite.setTextureRect(sf::IntRect(currentFrame * frameW, 0, frameW, frameH));
    }

    if (sprite.getPosition().y < -frameH) // off top of screen
        active = false;
}

void Knife::draw(sf::RenderWindow& window)
{
    if (active)
        window.draw(sprite);
}