#include "AssetManager.h"
#include <iostream>

// ---------------------------------------------------------------
// EXACT filenames the game looks for. Drop your images into the
// assets/ folder using these exact names and they'll appear automatically.
// ---------------------------------------------------------------
static const std::array<std::string, TETROMINO_COUNT> kFileNames = {
    "IMAGES/watermelontile.png", // I piece
    "IMAGES/sunflower.png",    // O piece
    "IMAGES/hibiscus.png",     // T piece
    "IMAGES/stars.png",        // S piece
    "IMAGES/headphones.png",   // Z piece
    "IMAGES/cassette.png",     // J piece
    "IMAGES/guitar.png"        // L piece
};

bool AssetManager::loadAll() {
    bool allOk = true;
    for (int i = 0; i < TETROMINO_COUNT; ++i) {
        if (textures_[i].loadFromFile(kFileNames[i])) {
            textures_[i].setSmooth(true);
            loaded_[i] = true;
        } else {
            loaded_[i] = false;
            allOk = false;
            std::cout << "[AssetManager] Missing " << kFileNames[i]
                      << " -> using placeholder color instead.\n";
        }
    }

    // Try a font for score/menu text. Put a .ttf in assets/ named font.ttf,
    // otherwise we fall back to common system font locations.
    if (font_.loadFromFile("assets/font.ttf")) {
        fontLoaded_ = true;
    } else if (font_.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        fontLoaded_ = true;
    } else if (font_.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        fontLoaded_ = true;
    } else {
        fontLoaded_ = false;
        std::cout << "[AssetManager] No font found - add assets/font.ttf for text to show.\n";
    }

    return allOk;
}

const sf::Texture* AssetManager::getTexture(TetrominoType type) const {
    int i = static_cast<int>(type);
    if (i < 0 || i >= TETROMINO_COUNT) return nullptr;
    if (!loaded_[i]) return nullptr;
    return &textures_[i];
}
