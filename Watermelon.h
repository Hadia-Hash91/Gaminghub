#pragma once
#include <SFML/Graphics.hpp>


class watermelon
{

private:
	sf::Texture watermelonTexture;
	sf::Sprite watermelonSprite;
		sf::Texture playgame1;
		sf::Sprite PLAYGAME1;

public:
	watermelon();
	void drawwatermelon(sf::RenderWindow& window);
	void drawwatermelon1(sf::RenderWindow& window);
};