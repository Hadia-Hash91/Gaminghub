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

    float speed = 500.f;   // upward speed
    bool active = false;

    // Same idea as Player: the strip frame is bigger than the actual knife
    // graphic, so trim the empty padding off before using it for collisions.
    // Tune these while watching the hitbox overlay (H key).
    float boundsTopTrim = 110.f;
    float boundsBottomTrim = 110.f;
    float boundsSideTrim = 15.f; // trims off BOTH the left and right edges

public:
    Knife();
    void fire(sf::Vector2f startPos);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool isActive() const { return active; }
    void deactivate() { active = false; }
    sf::FloatRect getBounds() const;
};