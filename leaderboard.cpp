#include"leaderboard.h"

Leaderboard::Leaderboard()
{
	leaderboardbackground.loadFromFile("IMAGES/LEADERBOARD.png");
	LEADERBOARDBACKGROUND.setTexture(leaderboardbackground);
	sf::Vector2u imagesize = leaderboardbackground.getSize();
	LEADERBOARDBACKGROUND.setScale(800.f / imagesize.x, 600.f / imagesize.y);
}

void Leaderboard::drawLeaderboard(sf::RenderWindow& window)
{
	window.draw(LEADERBOARDBACKGROUND);
}