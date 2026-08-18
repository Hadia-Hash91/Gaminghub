#pragma once
#include "Constants.h"
#include <array>
#include <string>

// Loads one texture per tetromino theme. If an image file is missing,
// that piece just falls back to a plain colored block instead of crashing -
// so the game runs today, and looks better the moment you drop images into assets/.
class AssetManager {
public:
    bool loadAll(); // returns true if ALL images loaded; game still runs either way
    const sf::Texture* getTexture(TetrominoType type) const; // nullptr if not loaded
    const sf::Font& getFont() const { return font_; }
    bool hasFont() const { return fontLoaded_; }

private:
    std::array<sf::Texture, TETROMINO_COUNT> textures_;
    std::array<bool, TETROMINO_COUNT> loaded_{};
    sf::Font font_;
    bool fontLoaded_ = false;
};
