#include "leaderboard.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

Leaderboard::Leaderboard()
{
	leaderboardbackground.loadFromFile("IMAGES/LEADERBOARD.png");
	LEADERBOARDBACKGROUND.setTexture(leaderboardbackground);
	sf::Vector2u imagesize = leaderboardbackground.getSize();
	if (imagesize.x > 0 && imagesize.y > 0)
		LEADERBOARDBACKGROUND.setScale(800.f / imagesize.x, 600.f / imagesize.y);

	font.loadFromFile("C:/Windows/Fonts/arial.ttf");
	reload();
}

void Leaderboard::reload()
{
	entries.clear();

	std::ifstream file("data/leaderboard.csv");
	if (!file.is_open()) return;

	std::string line;
	while (std::getline(file, line))
	{
		std::vector<std::string> parts;
		std::stringstream ss(line);
		std::string tok;
		while (std::getline(ss, tok, ',')) parts.push_back(tok);

		if (parts.size() < 5) continue;              // skip broken rows
		if (parts[0] != "LaneRunner") continue;      // skip other games' rows

		Entry e;
		if (parts.size() >= 6)
		{
			// current format: LaneRunner,name,score,coins,level,date
			e.name = parts[1];
			e.score = std::atoi(parts[2].c_str());
			e.coins = std::atoi(parts[3].c_str());
			e.level = std::atoi(parts[4].c_str());
			e.date = parts[5];
		}
		else
		{
			// older format: LaneRunner,score,coins,level,date
			e.name = "PLAYER";
			e.score = std::atoi(parts[1].c_str());
			e.coins = std::atoi(parts[2].c_str());
			e.level = std::atoi(parts[3].c_str());
			e.date = parts[4];
		}
		entries.push_back(e);
	}

	std::sort(entries.begin(), entries.end(),
		[](const Entry& a, const Entry& b) { return a.score > b.score; });
}

void Leaderboard::drawLeaderboard(sf::RenderWindow& window)
{
	window.draw(LEADERBOARDBACKGROUND);

	if (font.getInfo().family.empty()) return;

	sf::Text title("LANE RUNNER SCORES", font, 28);
	title.setFillColor(sf::Color::White);
	title.setOutlineColor(sf::Color::Black);
	title.setOutlineThickness(2.f);
	sf::FloatRect tb = title.getLocalBounds();
	title.setOrigin(tb.width / 2.f, tb.height / 2.f);
	title.setPosition(400.f, 50.f);
	window.draw(title);

	if (entries.empty())
	{
		sf::Text empty("No scores yet - play Lane Runner!", font, 18);
		empty.setFillColor(sf::Color(220, 220, 220));
		empty.setOutlineColor(sf::Color::Black);
		empty.setOutlineThickness(1.f);
		sf::FloatRect eb = empty.getLocalBounds();
		empty.setOrigin(eb.width / 2.f, eb.height / 2.f);
		empty.setPosition(400.f, 300.f);
		window.draw(empty);
		return;
	}

	size_t shown = std::min<size_t>(entries.size(), 10);
	float y = 120.f;
	for (size_t i = 0; i < shown; ++i)
	{
		std::string line = "#" + std::to_string(i + 1) + "  " + entries[i].name +
			"   Score: " + std::to_string(entries[i].score) +
			"   Coins: " + std::to_string(entries[i].coins) +
			"   Level: " + std::to_string(entries[i].level);

		sf::Text t(line, font, 17);
		t.setFillColor(i == 0 ? sf::Color(255, 215, 0) : sf::Color::White);
		t.setOutlineColor(sf::Color::Black);
		t.setOutlineThickness(1.f);
		sf::FloatRect b = t.getLocalBounds();
		t.setOrigin(b.width / 2.f, b.height / 2.f);
		t.setPosition(400.f, y);
		window.draw(t);
		y += 42.f;
	}

	sf::Text hint("Scores are saved automatically - clear them from Settings", font, 14);
	hint.setFillColor(sf::Color(180, 180, 180));
	hint.setOutlineColor(sf::Color::Black);
	hint.setOutlineThickness(1.f);
	sf::FloatRect hb = hint.getLocalBounds();
	hint.setOrigin(hb.width / 2.f, hb.height / 2.f);
	hint.setPosition(400.f, 575.f);
	window.draw(hint);
}
