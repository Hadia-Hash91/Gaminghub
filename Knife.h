#pragma once
#pragma once
#include <SFML/Graphics.hpp>

class Knife
{
private:
    sf::Texture texture;
    sf::Sprite sprite;

    int currentFrame = 0;
    float animTimer = 0.f;
    float frameDuration = 0.06f; // fast, since a knife throw is quick

    const int frameW = 707 / 8; /* fill in */;
    const int frameH = 353; /* fill in */;
    const int frameCount = 8;

    float speed = 200.f;   // upward speed, adjust to taste
    bool active = false;

public:
    Knife();
    void fire(sf::Vector2f startPos);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool isActive() const { return active; }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
};