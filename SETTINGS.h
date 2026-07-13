#pragma once
#include"Menu.h"

class settings
{
private:
	sf::Texture settingsbackground;
	sf::Sprite SETTINGSBACKGROUND;
public:
	settings();
	void drawSettings(sf::RenderWindow& window);
};