#pragma once

#include <SFML/Graphics.hpp>
#include"Menu.h"
#include"SETTINGS.h"
#include"Gameselection.h"
#include"leaderboard.h"
#include"Player.h"
#include"Watermelon.h"
#include "fallingw.h"
#include"Characterselection.h"
#include "TileTetris.h"
#include "LaneRunner.h"
const int MAX_WATERMELONS = 20;

enum class GameState
{
    MenuScreen,
    Playing,
    MainpageScreen,
    settingsScreen,
    GameSelectionScreen,
    LeaderboardScreen,
    Watermelon,
    HowToPlay1,
    GameOver,
    Characterselection,
    Tetris,
    LaneRunner

};
class Game
{
private:

    sf::RenderWindow window;
    sf::CircleShape circle;
    Menu menu;
    settings settingsScreen;
    GameSelection gameSelectionScreen;
    Leaderboard leaderboardScreen;
    bool isPressed = false;
    GameState state = GameState::MenuScreen;
    watermelon watermelonGame;
    Player player;
    Watermelon watermelons[MAX_WATERMELONS];
    sf::Texture backbutton;
    sf::Sprite BACKBUTTON;
    sf::Text livesText;
    chselection chsel;
    sf::Font font;
    sf::Text scoreText;
    TileTetris tetrisGame;
    LaneRunner laneRunnerGame;

    sf::Clock spawnClock;
    sf::Clock gameClock;
    int score = 0;

    bool showHitboxes = false; // toggled by pressing H

    void processEvents();
    void update();
    void render();
    void resetGame();
    void drawHitbox(sf::RenderWindow& window, const sf::FloatRect& rect, const sf::Color& color, float padding = 0.f);

public:
    // Constructor
    Game();
    int selectedCharacter = -1; // -1 = none chosen yet, 0/1/2 = character index
    // Main game loop
    void run();
};