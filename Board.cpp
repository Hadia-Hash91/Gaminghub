#include "Board.h"

Board::Board() {
    reset();
}

void Board::reset() {
    for (int r = 0; r < BOARD_HEIGHT; ++r)
        for (int c = 0; c < BOARD_WIDTH; ++c)
            grid_[r][c] = TetrominoType::None;
}

bool Board::isValidPosition(const Tetromino& piece) const {
    auto blocks = piece.getBlocks();
    for (const auto& b : blocks) {
        if (b.x < 0 || b.x >= BOARD_WIDTH) return false;      // outside left/right wall
        if (b.y < 0 || b.y >= BOARD_HEIGHT) return false;     // outside floor/ceiling
        if (grid_[b.y][b.x] != TetrominoType::None) return false; // overlaps a locked block
    }
    return true;
}

void Board::lockPiece(const Tetromino& piece) {
    auto blocks = piece.getBlocks();
    for (const auto& b : blocks) {
        if (b.y >= 0 && b.y < BOARD_HEIGHT && b.x >= 0 && b.x < BOARD_WIDTH) {
            grid_[b.y][b.x] = piece.getType();
        }
    }
}

int Board::clearFullLines() {
    int linesCleared = 0;

    for (int r = BOARD_HEIGHT - 1; r >= 0; --r) {
        bool full = true;
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            if (grid_[r][c] == TetrominoType::None) { full = false; break; }
        }

        if (full) {
            linesCleared++;
            // Shift every row above this one down by one.
            for (int rr = r; rr > 0; --rr) {
                grid_[rr] = grid_[rr - 1];
            }
            // Top row becomes empty.
            for (int c = 0; c < BOARD_WIDTH; ++c) {
                grid_[0][c] = TetrominoType::None;
            }
            // Re-check this same row index again since it now holds
            // whatever used to be one row above it.
            r++;
        }
    }

    return linesCleared;
}

bool Board::isTopRowBlocked() const {
    for (int c = 0; c < BOARD_WIDTH; ++c) {
        if (grid_[0][c] != TetrominoType::None) return true;
    }
    return false;
}

void Board::drawCell(sf::RenderWindow& window, const AssetManager& assets,
                      int col, int row, TetrominoType type) const {
    const sf::Texture* tex = assets.getTexture(type);

    if (tex) {
        drawSpriteFit(window, *tex, static_cast<float>(col * CELL_SIZE),
                      static_cast<float>(row * CELL_SIZE), CELL_SIZE);
    } else {
        sf::RectangleShape rect(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
        rect.setPosition(static_cast<float>(col * CELL_SIZE + 1), static_cast<float>(row * CELL_SIZE + 1));
        rect.setFillColor(fallbackColor(type));
        rect.setOutlineThickness(1.f);
        rect.setOutlineColor(sf::Color(0, 0, 0, 120));
        window.draw(rect);
    }
}

void Board::draw(sf::RenderWindow& window, const AssetManager& assets) const {
    // Board background / grid lines
    sf::RectangleShape bg(sf::Vector2f(BOARD_WIDTH * CELL_SIZE, BOARD_HEIGHT * CELL_SIZE));
    bg.setPosition(0, 0);
    bg.setFillColor(sf::Color(20, 20, 25, 150));
    window.draw(bg);

    for (int r = 0; r < BOARD_HEIGHT; ++r) {
        for (int c = 0; c < BOARD_WIDTH; ++c) {
            if (grid_[r][c] != TetrominoType::None) {
                drawCell(window, assets, c, r, grid_[r][c]);
            }
        }
    }
}
