#include "fallingw.h"

Watermelon::Watermelon()
{
    fallTexture.loadFromFile("IMAGES/wfalling_strip.png");
    cutTexture.loadFromFile("IMAGES/cutw.png");

    sprite.setTexture(fallTexture);
    sprite.setTextureRect(sf::IntRect(0, 0, fallFrameW, fallFrameH));
    sprite.setOrigin(fallFrameW / 2.f, fallFrameH / 2.f);
}

void Watermelon::spawn(float xPosition)
{
    active = true;
    isCut = false;
    currentFrame = 0;
    animTimer = 0.f;

    sprite.setTexture(fallTexture);
    sprite.setTextureRect(sf::IntRect(0, 0, fallFrameW, fallFrameH));
    sprite.setOrigin(fallFrameW / 2.f, fallFrameH / 2.f);
    sprite.setScale(1.f, 1.f);   // NEW: reset size back to normal
    sprite.setPosition(xPosition, -50.f);
}

void Watermelon::update(float dt)
{
    if (!active) return;

    if (!isCut)
    {
        sprite.move(0.f, fallSpeed * dt);

        animTimer += dt;
        if (animTimer >= frameDuration)
        {
            animTimer = 0.f;
            if (currentFrame < fallFrameCount - 1)
            {
                currentFrame++;
                sprite.setTextureRect(sf::IntRect(currentFrame * fallFrameW, 0, fallFrameW, fallFrameH));
            }
        }

        if (sprite.getPosition().y > 700.f)
            active = false;
    }
    else // cut - play cut animation AND keep falling
    {
        sprite.move(0.f, cutFallSpeed * dt);   // NEW: keep moving down

        animTimer += dt;
        if (animTimer >= frameDuration)
        {
            animTimer = 0.f;
            currentFrame++;
            if (currentFrame >= cutFrameCount)
            {
                active = false;
            }
            else
            {
                sprite.setTextureRect(sf::IntRect(currentFrame * cutFrameW, 0, cutFrameW, cutFrameH));
            }
        }

        if (sprite.getPosition().y > 700.f)   // NEW: also deactivate if it falls off screen
            active = false;
    }
}

void Watermelon::draw(sf::RenderWindow& window)
{
    if (active)
        window.draw(sprite);
}

void Watermelon::cut()
{
    isCut = true;
    currentFrame = 0;
    animTimer = 0.f;

    sprite.setTexture(cutTexture);
    sprite.setTextureRect(sf::IntRect(0, 0, cutFrameW, cutFrameH));
    sprite.setOrigin(cutFrameW / 2.f, cutFrameH / 2.f);
}