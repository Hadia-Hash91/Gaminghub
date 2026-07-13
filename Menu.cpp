#include"Menu.h"

Menu::Menu()
{

	menu.loadFromFile("IMAGES/startpage.png");
	MENU.setTexture(menu);
	startbutton.loadFromFile("IMAGES/STARTBUTTON.png");
	STARTBUTTON.setTexture(startbutton);
	STARTBUTTON.setPosition(350.f, 460.f); // Set the position of the start button
	STARTBUTTON.setScale(0.3f, 0.3f);  // 30% of original size
	mainpage.loadFromFile("IMAGES/MAINPAGE.png");
		MAINPAGE.setTexture(mainpage);
	
		mainstartbutton.loadFromFile("IMAGES/STBUTTON.png");
		MAINSTARTBUTTON.setTexture(mainstartbutton);
		MAINSTARTBUTTON.setPosition(250.f, 200.f); // Set the position of the start button
		MAINSTARTBUTTON.setScale(0.5f, 0.5f);
		settingbutton.loadFromFile("IMAGES/SETTINGSBUTTON.png");
		SETTINGBUTTON.setTexture(settingbutton);
        SETTINGBUTTON.setPosition(250.f, 260.f); // Set the position of the start button
		SETTINGBUTTON.setScale(0.5f, 0.5f);
		leaderboardbutton.loadFromFile("IMAGES/LEADERBOARDBUTTON.png");
		LEADERBOARDBUTTON.setTexture(leaderboardbutton);
		LEADERBOARDBUTTON.setPosition(250.f, 330.f); // Set the position of the start button
		LEADERBOARDBUTTON.setScale(0.5f, 0.5f);

	sf::Vector2u imagesize = menu.getSize();
	MENU.setScale(800.f / imagesize.x, 600.f / imagesize.y);
	sf::Vector2u imagES = mainpage.getSize();
	MAINPAGE.setScale(800.f / imagES.x, 600.f / imagES.y);

}
bool Menu::isStartButtonClicked(sf::Vector2f mousepos) 
{
	return STARTBUTTON.getGlobalBounds().contains(mousepos);
	
}
void Menu::ispressed()
{
	
			STARTBUTTON.setScale(0.27f, 0.27f);
	

}
void Menu::isreleased()
{
	STARTBUTTON.setScale(0.3f, 0.3f);
}

// Menu.cpp — add these
void Menu::updateMainStartHover(sf::Vector2f mousepos)
{
	updateHover(MAINSTARTBUTTON, mousepos);
}

void Menu::updateSettingsHover(sf::Vector2f mousepos)
{
	updateHover(SETTINGBUTTON, mousepos);
}

void Menu::updateLeaderboardHover(sf::Vector2f mousepos)
{
	updateHover(LEADERBOARDBUTTON, mousepos);
}
void Menu::updateHover(sf::Sprite& button, sf::Vector2f mousepos)
{
	if (button.getGlobalBounds().contains(mousepos))
		button.setColor(sf::Color(180, 180, 180)); // dimmed = hovered
	else
		button.setColor(sf::Color::White); // normal
}
bool Menu::isMainStartClicked(sf::Vector2f mousepos)
{
	return MAINSTARTBUTTON.getGlobalBounds().contains(mousepos);
}

bool Menu::isSettingsClicked(sf::Vector2f mousepos)
{
	return SETTINGBUTTON.getGlobalBounds().contains(mousepos);
}

bool Menu::isLeaderboardClicked(sf::Vector2f mousepos)
{
	return LEADERBOARDBUTTON.getGlobalBounds().contains(mousepos);
}

void Menu::drawMenu(sf::RenderWindow& window)
{
	window.draw(MENU);
	window.draw(STARTBUTTON);
}

void Menu::drawMainpage(sf::RenderWindow& window)
{
	window.draw(MAINPAGE);
	window.draw(MAINSTARTBUTTON);
	window.draw(SETTINGBUTTON);
	window.draw(LEADERBOARDBUTTON);
}