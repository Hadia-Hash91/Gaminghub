#include"Watermelon.h"

watermelon::watermelon()
{
	watermelonTexture.loadFromFile("IMAGES/GAMEWATERMELON.png");
	watermelonSprite.setTexture(watermelonTexture);
	playgame1.loadFromFile("IMAGES/waterplay.png");
	PLAYGAME1.setTexture(playgame1);
	sf::Vector2u imagesize = watermelonTexture.getSize();
	watermelonSprite.setScale(800.f / imagesize.x, 600.f / imagesize.y);

	sf::Vector2u imagesize1 = playgame1.getSize();
	PLAYGAME1.setScale(800.f / imagesize1.x, 600.f / imagesize1.y);
}

void watermelon::drawwatermelon(sf::RenderWindow& window)
{
	window.draw(watermelonSprite);
	

}

void watermelon::drawwatermelon1(sf::RenderWindow& window)
{
	window.draw(PLAYGAME1);
}