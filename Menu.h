#pragma once
#include <SFML/Graphics.hpp>
class Menu
{
private:
	sf::Texture menu;
	sf::Sprite MENU;
	sf::Texture startbutton;
	sf::Sprite STARTBUTTON;
	sf::Texture mainpage;
	sf::Sprite MAINPAGE;
	sf::Texture mainstartbutton;
	sf::Sprite MAINSTARTBUTTON;
	sf::Texture settingbutton;
	sf::Sprite SETTINGBUTTON;
	sf::Texture leaderboardbutton;
	sf::Sprite LEADERBOARDBUTTON;
	sf::Texture characterselectionbutton;
	sf::Sprite CHARACTERSELECTIONBUTTON;
	sf::Texture Exitbutton;
	sf::Sprite EXITBUTTON;

public:
	Menu();
	void drawMenu(sf::RenderWindow& window);
	void drawMainpage(sf::RenderWindow& window);
	bool isStartButtonClicked(sf::Vector2f mousepos);
	void ispressed();
	void isreleased();

	bool isMainStartClicked(sf::Vector2f mousepos);
	bool isSettingsClicked(sf::Vector2f mousepos);
	bool isLeaderboardClicked(sf::Vector2f mousepos);
	bool ischaracterselectionClicked(sf::Vector2f mousepos);
	bool isExitClicked(sf::Vector2f mousepos);
	void updateMainStartHover(sf::Vector2f mousepos);
	void updateSettingsHover(sf::Vector2f mousepos);
	void updateLeaderboardHover(sf::Vector2f mousepos);
	void updateHover(sf::Sprite& button, sf::Vector2f mousepos);
	void updatecharacterselectionHover(sf::Vector2f mousepos);
	void updateExitHover(sf::Vector2f mousepos);


};