#pragma once
#include "Constants.h"
#include "Board.h"
#include "Tetromino.h"
#include "AssetManager.h"
#include <SFML/Graphics.hpp>
#include <random>

// Tile Tetris - runs inside the hub's shared window. Call handleEvent/update/render
// from the hub's own game loop; it never creates or displays its own window.
class TileTetris {
public:
    explicit TileTetris(sf::RenderWindow& window);

    void reset();                      // back to the internal menu
    void handleEvent(const sf::Event& event);
    void update(float dt);
    void render();

    bool exitRequested() const { return exitRequested_; }
    void clearExitRequested() { exitRequested_ = false; }

private:
    sf::RenderWindow& window_;
    AssetManager assets_;
    Board board_;
    sf::Texture bgTexture_;
    bool bgLoaded_ = false;

    TetrisState state_;
    TetrisDifficulty difficulty_;

    Tetromino current_;
    Tetromino next_;

    int score_;
    int linesClearedTotal_;
    int level_;

    float dropTimer_;
    float dropInterval_; // seconds between automatic downward moves

    std::mt19937 rng_;

    bool exitRequested_ = false;

    // --- gameplay ---
    Tetromino makeRandomPiece();
    void spawnNewPiece();
    bool tryMove(int dx, int dy);
    void tryRotate();
    void hardDrop();
    void lockCurrentPiece();
    void startGame(TetrisDifficulty diff);
    void updateDropSpeed();

    // --- rendering ---
    void renderMenu();
    void renderHUD();
    void renderNextPreview();
    void renderGameOver();
    void renderPaused();

    // Difficulty -> base falling speed
    float baseDropIntervalFor(TetrisDifficulty d) const;
};
