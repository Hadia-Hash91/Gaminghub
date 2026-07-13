#pragma once

#include <SFML/Graphics.hpp>
#include"Menu.h"
#include"SETTINGS.h"
#include"Gameselection.h"
#include"leaderboard.h"
enum class GameState
{
    MenuScreen,
    Playing,
	MainpageScreen,
	settingsScreen,
	GameSelectionScreen,
	LeaderboardScreen,
    PurplePlacePlaying,
    chess,
    HowToPlay1
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

    void processEvents();
    void update();
    void render();

public:
    // Constructor
    Game();

    // Main game loop
    void run();
};