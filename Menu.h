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

	void updateMainStartHover(sf::Vector2f mousepos);
	void updateSettingsHover(sf::Vector2f mousepos);
	void updateLeaderboardHover(sf::Vector2f mousepos);
	void updateHover(sf::Sprite& button, sf::Vector2f mousepos);

};