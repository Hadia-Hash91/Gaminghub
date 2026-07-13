#include"SETTINGS.h"


settings::settings()
{
	settingsbackground.loadFromFile("IMAGES/SETTING.png");
	SETTINGSBACKGROUND.setTexture(settingsbackground);
	sf::Vector2u imagesize = settingsbackground.getSize();
	SETTINGSBACKGROUND.setScale(800.f / imagesize.x, 600.f / imagesize.y);
}

void settings::drawSettings(sf::RenderWindow& window)
{
	window.draw(SETTINGSBACKGROUND);
}