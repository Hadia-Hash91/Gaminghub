#include "SETTINGS.h"
#include <fstream>
#include <sstream>

settings::settings()
{
	settingsbackground.loadFromFile("IMAGES/SETTING.png");
	SETTINGSBACKGROUND.setTexture(settingsbackground);
	sf::Vector2u imagesize = settingsbackground.getSize();
	if (imagesize.x > 0 && imagesize.y > 0)
		SETTINGSBACKGROUND.setScale(800.f / imagesize.x, 600.f / imagesize.y);

	font.loadFromFile("C:/Windows/Fonts/arial.ttf");

	// clickable rows
	nameBox = sf::FloatRect(250.f, 220.f, 300.f, 50.f);
	diffBox = sf::FloatRect(250.f, 300.f, 300.f, 50.f);
	clearBox = sf::FloatRect(250.f, 400.f, 300.f, 50.f);

	load();
}

void settings::load()
{
	playerName = "PLAYER";
	difficulty = 1;

	std::ifstream file("data/settings.txt");
	if (!file.is_open()) return;

	std::string key, value;
	while (file >> key >> value)
	{
		if (key == "name" && !value.empty()) playerName = value;
		else if (key == "difficulty")
		{
			std::stringstream ss(value);
			int d = 1;
			ss >> d;
			if (d >= 0 && d <= 2) difficulty = d;
		}
	}
}

void settings::save()
{
	std::ofstream file("data/settings.txt");
	if (!file.is_open()) return;
	file << "name " << playerName << "\n";
	file << "difficulty " << difficulty << "\n";
}

std::string diffName(int d)
{
	return d == 0 ? "EASY" : (d == 2 ? "HARD" : "NORMAL");
}

void settings::handleEvent(const sf::Event& event, sf::Vector2f mousePos)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (nameBox.contains(mousePos))
		{
			editingName = !editingName;
		}
		else if (diffBox.contains(mousePos))
		{
			editingName = false;
			difficulty = (difficulty + 1) % 3;
			save();
		}
		else if (clearBox.contains(mousePos))
		{
			editingName = false;
			clearLeaderboard();
		}
		else
		{
			editingName = false;   // clicked somewhere else - stop editing
		}
	}

	if (!editingName) return;

	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode >= 32 && event.text.unicode < 127)
		{
			char ch = static_cast<char>(event.text.unicode);
			if (ch != ',' && ch != ' ' && playerName.size() < 12)
			{
				playerName += ch;
				save();
			}
		}
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Backspace && !playerName.empty())
		{
			playerName.pop_back();
			save();
		}
		else if (event.key.code == sf::Keyboard::Enter)
		{
			editingName = false;
			save();
		}
	}
}

void settings::clearLeaderboard()
{
	std::ofstream file("data/leaderboard.csv", std::ios::trunc);
	file.close();
}

void settings::drawSettings(sf::RenderWindow& window)
{
	window.draw(SETTINGSBACKGROUND);

	if (font.getInfo().family.empty()) return;

	sf::Text title("SETTINGS", font, 40);
	title.setFillColor(sf::Color::White);
	title.setOutlineColor(sf::Color::Black);
	title.setOutlineThickness(3.f);
	sf::FloatRect tb = title.getLocalBounds();
	title.setOrigin(tb.width / 2.f, tb.height / 2.f);
	title.setPosition(400.f, 90.f);
	window.draw(title);

	// Name row
	sf::Text name("Name: " + playerName + (editingName ? "_" : ""), font, 24);
	name.setFillColor(editingName ? sf::Color(120, 255, 120) : sf::Color::White);
	name.setOutlineColor(sf::Color::Black);
	name.setOutlineThickness(2.f);
	sf::FloatRect nb = name.getLocalBounds();
	name.setOrigin(nb.width / 2.f, nb.height / 2.f);
	name.setPosition(400.f, 245.f);
	window.draw(name);

	// Difficulty row
	sf::Text diff("Difficulty: " + diffName(difficulty), font, 24);
	diff.setFillColor(sf::Color::White);
	diff.setOutlineColor(sf::Color::Black);
	diff.setOutlineThickness(2.f);
	sf::FloatRect db = diff.getLocalBounds();
	diff.setOrigin(db.width / 2.f, db.height / 2.f);
	diff.setPosition(400.f, 325.f);
	window.draw(diff);

	// Clear leaderboard row
	sf::Text clear("CLEAR LEADERBOARD", font, 24);
	clear.setFillColor(sf::Color(255, 120, 120));
	clear.setOutlineColor(sf::Color::Black);
	clear.setOutlineThickness(2.f);
	sf::FloatRect cb = clear.getLocalBounds();
	clear.setOrigin(cb.width / 2.f, cb.height / 2.f);
	clear.setPosition(400.f, 425.f);
	window.draw(clear);

	sf::Text hint("Click a row to change it  -  ESC/back button to leave", font, 16);
	hint.setFillColor(sf::Color(200, 200, 200));
	hint.setOutlineColor(sf::Color::Black);
	hint.setOutlineThickness(1.f);
	sf::FloatRect hb = hint.getLocalBounds();
	hint.setOrigin(hb.width / 2.f, hb.height / 2.f);
	hint.setPosition(400.f, 520.f);
	window.draw(hint);
}
