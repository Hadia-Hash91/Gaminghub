#pragma once
#include"Menu.h"

class Leaderboard
{
private:
	sf::Texture leaderboardbackground;
	sf::Sprite LEADERBOARDBACKGROUND;

public:
	Leaderboard();
	void drawLeaderboard(sf::RenderWindow& window);
};