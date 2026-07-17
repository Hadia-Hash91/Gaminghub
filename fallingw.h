#pragma once
#include <SFML/Graphics.hpp>

class Watermelon
{
private:
    sf::Texture fallTexture, cutTexture;
    sf::Sprite sprite;

    int currentFrame = 0;
    float animTimer = 0.f;
    float frameDuration = 0.1f;

    const int fallFrameW =612/9/* fill in */;
    const int fallFrameH = 408/* fill in */;
    const int fallFrameCount = 9;

    const int cutFrameW = 707/6;
    const int cutFrameH = 353;
    const int cutFrameCount = 6/* fill in, however many frames the cut strip has */;

    float cutFallSpeed = 250.f;   // NEW: falls a bit faster once cut

    float fallSpeed = 100.f;
    bool active = false;
    bool isCut = false;   // NEW: are we currently playing the cut animation?

public:
    Watermelon();
    void spawn(float xPosition);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void cut();
    bool getIsCut() const { return isCut; }
    bool isActive() const { return active; }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
};