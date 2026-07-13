#include"Gameselection.h"

GameSelection::GameSelection()
{
	gameselectionbackground.loadFromFile("IMAGES/GAMESELECTION.png");
	GAMESELECTIONBACKGROUND.setTexture(gameselectionbackground);
	sf::Vector2u imagesize = gameselectionbackground.getSize();
	GAMESELECTIONBACKGROUND.setScale(800.f / imagesize.x, 600.f / imagesize.y);
	purpleplace.loadFromFile("IMAGES/PURPLEPLACE.png");
	PURPLEPLACE.setTexture(purpleplace);
	PURPLEPLACE.setScale(0.4f, 0.4f);
	PURPLEPLACE.setPosition(100.f, 270.f);

	chess.loadFromFile("IMAGES/CHESS.png");
	CHESS.setTexture(chess);
	CHESS.setScale(0.6f, 0.6f);
	CHESS.setPosition(265.f, 260.f);

	howtoplay1.loadFromFile("IMAGES/CHESS.png");
	HOWTOPLAY1.setTexture(howtoplay1);
	HOWTOPLAY1.setScale(0.3f, 0.3f);
	HOWTOPLAY1.setPosition(265.f, 200.f);
	
}

void GameSelection::drawGameSelection(sf::RenderWindow& window)
{
	window.draw(GAMESELECTIONBACKGROUND);
	window.draw(PURPLEPLACE);
	window.draw(CHESS);
}

// GameSelection.cpp
bool GameSelection::ispurpleplaceclicked(sf::Vector2f mousepos)
{
	return PURPLEPLACE.getGlobalBounds().contains(mousepos);
}

void GameSelection::updatePurplePlaceHover(sf::Vector2f mousepos)
{
	if (PURPLEPLACE.getGlobalBounds().contains(mousepos))
		PURPLEPLACE.setColor(sf::Color(180, 180, 180));
	else
		PURPLEPLACE.setColor(sf::Color::White);
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