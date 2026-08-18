#pragma once

#include <SFML/Graphics.hpp>

// Character selection screen. Shows 3 characters per page, with a NEXT button
// on page 1 to reach 3 more slots (page 2). Characters 4-6 use placeholder
// boxes until you add ch4_strip1.png / ch5_strip1.png / ch6_strip1.png.
class chselection
{
private:
	sf::Texture cselection;
	sf::Sprite CSELECTION;

	sf::Texture stripTex[6];
	sf::Sprite strips[6];
	bool stripLoaded[6] = {};

	sf::Texture buttonTex[3];
	sf::Sprite selectButtons[3];

	sf::Texture nextPageTex;
	sf::Sprite NEXT_PAGE;
	sf::Sprite PREV_PAGE;

	sf::Font font;

	int page = 0;
	int currentFrame = 0;
	float animTimer = 0.f;
	float frameDuration = 0.2f;

	int selected = -1;

	const float cardX[3] = { 530.f, 100.f, 300.f };
	const float cardY = 160.f;
	const float buttonX[3] = { 530.f, 100.f, 300.f };
	const float buttonY = 470.f;

public:
	chselection();
	void draw(sf::RenderWindow& window);
	void update(float dt);
	int handleClick(sf::Vector2f mousePos);
	bool isNextPageClicked(sf::Vector2f mousePos);
	bool isPrevPageClicked(sf::Vector2f mousePos);
	void nextPage() { if (page < 1) page++; }
	void prevPage() { if (page > 0) page--; }
	void setSelected(int s) { selected = s; }
	int getSelected() const { return selected; }
};
