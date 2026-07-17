#pragma once

#include <SFML/Graphics.hpp>
#include"Menu.h"
#include"SETTINGS.h"
#include"Gameselection.h"
#include"leaderboard.h"
#include"Player.h"
#include"Watermelon.h"
#include "fallingw.h"

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
    chess,
    HowToPlay1,
    GameOver
	
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

     sf::Text livesText;

     sf::Font font;
     sf::Text scoreText;

     sf::Clock spawnClock;
     sf::Clock gameClock;
     int score = 0;
    void processEvents();
    void update();
    void render();

public:
    // Constructor
    Game();

    // Main game loop
    void run();
};