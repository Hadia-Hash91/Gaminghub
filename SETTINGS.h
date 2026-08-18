#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Fully functional settings screen:
//   - your player name (used on the leaderboard)
//   - difficulty (Easy / Normal / Hard) - affects Lane Runner speed
//   - clear the leaderboard
// Everything is saved to data/settings.txt so it persists between runs.
class settings
{
private:
	sf::Texture settingsbackground;
	sf::Sprite SETTINGSBACKGROUND;
	sf::Font font;

	std::string playerName;
	int difficulty = 1;             // 0 Easy, 1 Normal, 2 Hard
	bool editingName = false;
	bool mouseDown = false;

	sf::FloatRect nameBox;
	sf::FloatRect diffBox;
	sf::FloatRect clearBox;

	void load();
	void save();

public:
	settings();
	void handleEvent(const sf::Event& event, sf::Vector2f mousePos);
	void drawSettings(sf::RenderWindow& window);

	std::string getPlayerName() const { return playerName.empty() ? "PLAYER" : playerName; }
	int getDifficulty() const { return difficulty; }
	void clearLeaderboard();
};
