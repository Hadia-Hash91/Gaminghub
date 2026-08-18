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

	tetrisgamebox.loadFromFile("IMAGES/tetrisgamebox.png");
	TETRISGAMEBOX.setTexture(tetrisgamebox);
	TETRISGAMEBOX.setScale(0.25f, 0.25f);
	TETRISGAMEBOX.setPosition(265.f, 260.f);

	howtoplay1.loadFromFile("IMAGES/HOWTOPLAY1.png");
	HOWTOPLAY1.setTexture(howtoplay1);
	HOWTOPLAY1.setScale(0.25f, 0.25f);
	HOWTOPLAY1.setPosition(120.f, 370.f);

	runnerFont.loadFromFile("C:/Windows/Fonts/arial.ttf");
	LANERUNNERBUTTON.setSize(sf::Vector2f(160.f, 70.f));
	LANERUNNERBUTTON.setPosition(590.f, 200.f);
	LANERUNNERBUTTON.setFillColor(sf::Color(30, 120, 200));
	LANERUNNERBUTTON.setOutlineColor(sf::Color::White);
	LANERUNNERBUTTON.setOutlineThickness(2.f);

	LANERUNNERLABEL.setFont(runnerFont);
	LANERUNNERLABEL.setString("LANE\nRUNNER");
	LANERUNNERLABEL.setCharacterSize(18);
	LANERUNNERLABEL.setFillColor(sf::Color::White);
	LANERUNNERLABEL.setOutlineColor(sf::Color::Black);
	LANERUNNERLABEL.setOutlineThickness(1.f);
	sf::FloatRect labelBounds = LANERUNNERLABEL.getLocalBounds();
	LANERUNNERLABEL.setOrigin(labelBounds.width / 2.f, labelBounds.height / 2.f);
	LANERUNNERLABEL.setPosition(590.f + 80.f, 200.f + 35.f);
}

void GameSelection::drawGameSelection(sf::RenderWindow& window)
{
	window.draw(GAMESELECTIONBACKGROUND);
	window.draw(WATERMELONSHOOTER);
	window.draw(TETRISGAMEBOX);
	window.draw(HOWTOPLAY1);
	window.draw(LANERUNNERBUTTON);
	window.draw(LANERUNNERLABEL);
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

bool GameSelection::isTetrisGameBoxClicked(sf::Vector2f mousepos)
{
	return TETRISGAMEBOX.getGlobalBounds().contains(mousepos);
}

void GameSelection::updateTetrisGameBoxHover(sf::Vector2f mousepos)
{
	if (TETRISGAMEBOX.getGlobalBounds().contains(mousepos))
		TETRISGAMEBOX.setColor(sf::Color(180, 180, 180));
	else
		TETRISGAMEBOX.setColor(sf::Color::White);
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

bool GameSelection::isLaneRunnerClicked(sf::Vector2f mousepos)
{
	return LANERUNNERBUTTON.getGlobalBounds().contains(mousepos);
}

void GameSelection::updateLaneRunnerHover(sf::Vector2f mousepos)
{
	if (LANERUNNERBUTTON.getGlobalBounds().contains(mousepos))
		LANERUNNERBUTTON.setFillColor(sf::Color(60, 160, 240));
	else
		LANERUNNERBUTTON.setFillColor(sf::Color(30, 120, 200));
}