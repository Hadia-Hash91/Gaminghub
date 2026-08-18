#include "Tetromino.h"

// Index order matches TetrominoType enum: I, O, T, S, Z, J, L
const std::array<std::array<std::array<BlockOffset, 4>, 4>, TETROMINO_COUNT> Tetromino::shapeData_ = {{
    // I - watermelon
    {{
        {{ {0,1},{1,1},{2,1},{3,1} }},
        {{ {2,0},{2,1},{2,2},{2,3} }},
        {{ {0,2},{1,2},{2,2},{3,2} }},
        {{ {1,0},{1,1},{1,2},{1,3} }}
    }},
    // O - sunflower (same every rotation)
    {{
        {{ {1,0},{2,0},{1,1},{2,1} }},
        {{ {1,0},{2,0},{1,1},{2,1} }},
        {{ {1,0},{2,0},{1,1},{2,1} }},
        {{ {1,0},{2,0},{1,1},{2,1} }}
    }},
    // T - hibiscus
    {{
        {{ {0,1},{1,1},{2,1},{1,0} }},
        {{ {1,0},{1,1},{1,2},{2,1} }},
        {{ {0,1},{1,1},{2,1},{1,2} }},
        {{ {1,0},{1,1},{1,2},{0,1} }}
    }},
    // S - stars
    {{
        {{ {1,0},{2,0},{0,1},{1,1} }},
        {{ {1,0},{1,1},{2,1},{2,2} }},
        {{ {1,0},{2,0},{0,1},{1,1} }},
        {{ {1,0},{1,1},{2,1},{2,2} }}
    }},
    // Z - headphones
    {{
        {{ {0,0},{1,0},{1,1},{2,1} }},
        {{ {2,0},{1,1},{2,1},{1,2} }},
        {{ {0,0},{1,0},{1,1},{2,1} }},
        {{ {2,0},{1,1},{2,1},{1,2} }}
    }},
    // J - cassette
    {{
        {{ {0,0},{0,1},{1,1},{2,1} }},
        {{ {1,0},{2,0},{1,1},{1,2} }},
        {{ {0,1},{1,1},{2,1},{2,2} }},
        {{ {1,0},{1,1},{1,2},{0,2} }}
    }},
    // L - guitar
    {{
        {{ {2,0},{0,1},{1,1},{2,1} }},
        {{ {1,0},{1,1},{1,2},{2,2} }},
        {{ {0,1},{1,1},{2,1},{0,2} }},
        {{ {0,0},{1,0},{1,1},{1,2} }}
    }}
}};

Tetromino::Tetromino(TetrominoType type)
    : type_(type), x_(BOARD_WIDTH / 2 - 2), y_(0), rotation_(0) {
}

std::array<sf::Vector2i, 4> Tetromino::getBlocks() const {
    std::array<sf::Vector2i, 4> blocks;
    int typeIndex = static_cast<int>(type_);
    const auto& offsets = shapeData_[typeIndex][rotation_];
    for (int i = 0; i < 4; ++i) {
        blocks[i] = sf::Vector2i(x_ + offsets[i].x, y_ + offsets[i].y);
    }
    return blocks;
}

void Tetromino::moveBy(int dx, int dy) {
    x_ += dx;
    y_ += dy;
}

void Tetromino::rotateClockwise() {
    rotation_ = (rotation_ + 1) % 4;
}

void Tetromino::undoRotate() {
    rotation_ = (rotation_ + 3) % 4; // same as -1 mod 4
}
