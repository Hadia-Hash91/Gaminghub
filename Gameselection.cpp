#include"Gameselection.h"

GameSelection::GameSelection()
{
	gameselectionbackground.loadFromFile("IMAGES/Gameselection.png");
	GAMESELECTIONBACKGROUND.setTexture(gameselectionbackground);
	sf::Vector2u imagesize = gameselectionbackground.getSize();
	GAMESELECTIONBACKGROUND.setScale(800.f / imagesize.x, 600.f / imagesize.y);
	watermelonshooter.loadFromFile("IMAGES/Watermelonshooter.png");
	WATERMELONSHOOTER.setTexture(watermelonshooter);
	WATERMELONSHOOTER.setScale(0.6f, 0.6f);
	WATERMELONSHOOTER.setPosition(100.f, 210.f);

	chess.loadFromFile("IMAGES/CHESS.png");
	CHESS.setTexture(chess);
	CHESS.setScale(0.6f, 0.6f);
	CHESS.setPosition(265.f, 260.f);

	howtoplay1.loadFromFile("IMAGES/HOWTOPLAY1.png");
	HOWTOPLAY1.setTexture(howtoplay1);
	HOWTOPLAY1.setScale(0.25f, 0.25f);
	HOWTOPLAY1.setPosition(120.f, 370.f);
	
	
}

void GameSelection::drawGameSelection(sf::RenderWindow& window)
{
	window.draw(GAMESELECTIONBACKGROUND);
	window.draw(WATERMELONSHOOTER);
	window.draw(CHESS);
	window.draw(HOWTOPLAY1);
}

// GameSelection.cpp
bool GameSelection::iswatermelonclicked(sf::Vector2f mousepos)
{
	return WATERMELONSHOOTER.getGlobalBounds().contains(mousepos);
}

void GameSelection::updatewatermelonHover(sf::Vector2f mousepos)
{
	if (WATERMELONSHOOTER.getGlobalBounds().contains(mousepos))
		WATERMELONSHOOTER.setColor(sf::Color(180, 180, 180));
	else
		WATERMELONSHOOTER.setColor(sf::Color::White);
}

bool GameSelection::ischessclicked(sf::Vector2f mousepos)
{
	return CHESS.getGlobalBounds().contains(mousepos);
}

void GameSelection::updatechessHover(sf::Vector2f mousepos)
{
	if (CHESS.getGlobalBounds().contains(mousepos))
		CHESS.setColor(sf::Color(180, 180, 180));
	else
		CHESS.setColor(sf::Color::White);
}

bool GameSelection::ishowtoplay1clicked(sf::Vector2f mousepos)
{
	return HOWTOPLAY1.getGlobalBounds().contains(mousepos);
}

void GameSelection::updatehowtoplay1Hover(sf::Vector2f mousepos)
{
	if (HOWTOPLAY1.getGlobalBounds().contains(mousepos))
		HOWTOPLAY1.setColor(sf::Color(180, 180, 180));
	else
		HOWTOPLAY1.setColor(sf::Color::White);
}