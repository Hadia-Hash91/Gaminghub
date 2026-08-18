#pragma once
#include"Menu.h"
class GameSelection
{
private:
	sf::Texture gameselectionbackground;
	sf::Sprite GAMESELECTIONBACKGROUND;
	sf::Texture watermelonshooter;
	sf::Sprite WATERMELONSHOOTER;
	Menu menu1;
	sf::Texture tetrisgamebox;
	sf::Sprite TETRISGAMEBOX;
	sf::Texture howtoplay1;
	sf:: Sprite HOWTOPLAY1;

	sf::Font runnerFont;
	sf::RectangleShape LANERUNNERBUTTON;
	sf::Text LANERUNNERLABEL;
public:
	GameSelection();
	void drawGameSelection(sf::RenderWindow& window);
	
	bool iswatermelonclicked(sf::Vector2f mousepos);
	void updatewatermelonHover(sf::Vector2f mousepos);

	bool isTetrisGameBoxClicked(sf::Vector2f mousepos);
	void updateTetrisGameBoxHover(sf::Vector2f mousepos);

	bool ishowtoplay1clicked(sf::Vector2f mousepos);
	void updatehowtoplay1Hover(sf::Vector2f mousepos);

	bool isLaneRunnerClicked(sf::Vector2f mousepos);
	void updateLaneRunnerHover(sf::Vector2f mousepos);




};
