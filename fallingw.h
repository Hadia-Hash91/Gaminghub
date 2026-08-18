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

    const int fallFrameW = 612 / 9/* fill in */;
    const int fallFrameH = 408/* fill in */;
    const int fallFrameCount = 9;

    const int cutFrameW = 707 / 6;
    const int cutFrameH = 353;
    const int cutFrameCount = 6/* fill in, however many frames the cut strip has */;

    float cutFallSpeed = 250.f;   // NEW: falls a bit faster once cut

    float fallSpeed = 100.f;
    bool active = false;
    bool isCut = false;   // NEW: are we currently playing the cut animation?

    // The strip frames have empty padding above/below the actual fruit
    // (leaves room for motion blur / splash effects). Trim that off before
    // using it for collisions. Falling and cut frames are different sizes,
    // so they get separate trim amounts. Tune while watching H overlay.
    float fallBoundsTopTrim = 130.f;
    float fallBoundsBottomTrim = 130.f;
    float cutBoundsTopTrim = 100.f;
    float cutBoundsBottomTrim = 100.f;

public:
    Watermelon();
    void spawn(float xPosition);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void cut();
    void deactivate() { active = false; isCut = false; }
    void setFallSpeed(float speed) { fallSpeed = speed; }
    float getFallSpeed() const { return fallSpeed; }
    bool getIsCut() const { return isCut; }
    bool isActive() const { return active; }

    sf::FloatRect getBounds() const
    {
        sf::FloatRect b = sprite.getGlobalBounds();
        float topTrim = isCut ? cutBoundsTopTrim : fallBoundsTopTrim;
        float bottomTrim = isCut ? cutBoundsBottomTrim : fallBoundsBottomTrim;
        b.top += topTrim;
        b.height -= (topTrim + bottomTrim);
        return b;
    }
};