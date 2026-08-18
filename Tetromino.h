#pragma once
#include "Constants.h"
#include <array>

// A tetromino is made of 4 blocks. We describe each shape as offsets
// inside a 4x4 bounding box, one set of offsets per rotation state (0-3).
// This is the classic beginner-friendly way to do Tetris rotation:
// instead of math-rotating points, we just hardcode what each rotation looks like.

struct BlockOffset {
    int x;
    int y;
};

class Tetromino {
public:
    Tetromino(TetrominoType type);

    // Returns the 4 blocks in ABSOLUTE board coordinates (accounts for
    // current position + current rotation).
    std::array<sf::Vector2i, 4> getBlocks() const;

    void moveBy(int dx, int dy);
    void rotateClockwise();
    void undoRotate(); // used when a rotation turns out to be invalid

    TetrominoType getType() const { return type_; }
    int getX() const { return x_; }
    int getY() const { return y_; }
    int getRotation() const { return rotation_; }

    void setPosition(int x, int y) { x_ = x; y_ = y; }

private:
    TetrominoType type_;
    int x_, y_;        // top-left of the 4x4 bounding box, in board cells
    int rotation_;      // 0..3

    // shapeData_[type][rotation] = 4 block offsets
    static const std::array<std::array<std::array<BlockOffset, 4>, 4>, TETROMINO_COUNT> shapeData_;
};
