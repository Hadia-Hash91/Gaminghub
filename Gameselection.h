#pragma once
#include"Menu.h"
class GameSelection
{
private:
	sf::Texture gameselectionbackground;
	sf::Sprite GAMESELECTIONBACKGROUND;
	sf::Texture purpleplace;
	sf::Sprite PURPLEPLACE;
	Menu menu1;
	sf::Texture chess;
	sf::Sprite CHESS;
	sf::Texture howtoplay1;
	sf:: Sprite HOWTOPLAY1;
public:
	GameSelection();
	void drawGameSelection(sf::RenderWindow& window);
	
	bool ispurpleplaceclicked(sf::Vector2f mousepos);
	void updatePurplePlaceHover(sf::Vector2f mousepos);
	
	bool ischessclicked(sf::Vector2f mousepos);
	void updatechessHover(sf::Vector2f mousepos);

	bool ishowtoplay1clicked(sf::Vector2f mousepos);
	void updatehowtoplay1Hover(sf::Vector2f mousepos);




};
