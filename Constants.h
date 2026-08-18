#pragma once
#include <SFML/Graphics.hpp>
#include <array>

// ---------- Board dimensions ----------
constexpr int BOARD_WIDTH  = 10;   // columns
constexpr int BOARD_HEIGHT = 20;   // rows
constexpr int CELL_SIZE    = 25;   // pixels per grid cell

// Extra space on screen for score / next-piece preview panel
constexpr int SIDE_PANEL_WIDTH = 200;

constexpr int WINDOW_WIDTH  = BOARD_WIDTH * CELL_SIZE + SIDE_PANEL_WIDTH;
constexpr int WINDOW_HEIGHT = BOARD_HEIGHT * CELL_SIZE;

// ---------- The 7 classic tetromino shapes ----------
// Each one is tied to one of your tile designs.
enum class TetrominoType {
    I,      // Watermelon   (long row of 4 -> matches your watermelon strip)
    O,      // Sunflower    (2x2 square)
    T,      // Hibiscus     (T shape)
    S,      // Stars        (S shape)
    Z,      // Headphones   (Z shape)
    J,      // Cassette     (J shape)
    L,      // Guitar       (L shape)
    None    // empty cell on the board
};

constexpr int TETROMINO_COUNT = 7; // number of real types (excludes None)

// ---------- Difficulty ----------
enum class TetrisDifficulty {
    Easy,
    Medium,
    Hard
};

// ---------- Game state machine ----------
enum class TetrisState {
    Menu,       // difficulty selection screen
    Playing,
    Paused,
    GameOver
};

// Draws a texture stretched to fill the whole grid cell, edge to edge.
inline void drawSpriteFit(sf::RenderWindow& window, const sf::Texture& tex,
                          float cellX, float cellY, float cellSize) {
    sf::Sprite sprite(tex);
    sf::Vector2u size = tex.getSize();
    sprite.setScale(cellSize / static_cast<float>(size.x),
                    cellSize / static_cast<float>(size.y));
    sprite.setPosition(cellX, cellY);
    window.draw(sprite);
}

// Fallback colors, used only if a texture image hasn't been added yet.
// This means the game is fully playable before you drop in your art.
inline sf::Color fallbackColor(TetrominoType type) {
    switch (type) {
        case TetrominoType::I: return sf::Color(90, 190, 90);    // watermelon green
        case TetrominoType::O: return sf::Color(240, 200, 60);   // sunflower yellow
        case TetrominoType::T: return sf::Color(190, 90, 190);   // hibiscus purple
        case TetrominoType::S: return sf::Color(80, 150, 230);   // star blue
        case TetrominoType::Z: return sf::Color(120, 200, 150);  // headphones green
        case TetrominoType::J: return sf::Color(60, 110, 200);   // cassette blue
        case TetrominoType::L: return sf::Color(210, 70, 70);    // guitar red
        default: return sf::Color::Transparent;
    }
}
