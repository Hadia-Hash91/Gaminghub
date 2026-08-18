#pragma once
#include "Constants.h"
#include "Tetromino.h"
#include "AssetManager.h"
#include <array>

// The board owns a fixed-size grid (no dynamic containers needed - each
// cell just remembers which TetrominoType is locked there, or None if empty).
class Board {
public:
    Board();

    void reset();

    // Would this piece's position be legal (inside walls, not overlapping locked blocks)?
    bool isValidPosition(const Tetromino& piece) const;

    // Copy the piece's blocks permanently into the grid.
    void lockPiece(const Tetromino& piece);

    // Clears any full rows, shifts everything above down, and returns
    // how many rows were cleared (0-4).
    int clearFullLines();

    // Is the very top row occupied? Used to detect game over.
    bool isTopRowBlocked() const;

    void draw(sf::RenderWindow& window, const AssetManager& assets) const;

private:
    std::array<std::array<TetrominoType, BOARD_WIDTH>, BOARD_HEIGHT> grid_;

    void drawCell(sf::RenderWindow& window, const AssetManager& assets,
                  int col, int row, TetrominoType type) const;
};
