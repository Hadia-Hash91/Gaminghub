#pragma once
#include "Menu.h"
#include <string>
#include <vector>

// Shows the Lane Runner scoreboard. Rows are written to data/leaderboard.csv
// by the game as:  LaneRunner,name,score,coins,level,date
// This screen reads them back, sorts by score, and draws the top 10.
// Old rows without a name are shown as "PLAYER".
class Leaderboard
{
private:
	struct Entry
	{
		std::string name;
		int score;
		int coins;
		int level;
		std::string date;
	};

	sf::Texture leaderboardbackground;
	sf::Sprite LEADERBOARDBACKGROUND;
	sf::Font font;
	std::vector<Entry> entries;

public:
	Leaderboard();
	void reload();
	void drawLeaderboard(sf::RenderWindow& window);
};
