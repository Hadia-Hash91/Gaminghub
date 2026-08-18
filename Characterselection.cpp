#include "Characterselection.h"
#include <iostream>

namespace {
const int FRAME_COUNT = 6; // every strip has 6 animation frames

// Per-character strip dimensions: 0-2 use the known sizes, 3-5 are guessed
// from the image itself (width / 6 frames) once you add the art.
int frameWFor(int i) {
    switch (i) {
        case 0: return 666 / 6;
        case 1: return 707 / 6;
        case 2: return 725 / 6;
        default: return 0; // resolved at load time from the texture
    }
}
int frameHFor(int i) {
    switch (i) {
        case 0: return 375;
        case 1: return 353;
        case 2: return 344;
        default: return 0;
    }
}
}

chselection::chselection()
{
	if (!cselection.loadFromFile("images/CHARACTERSELECTION.png"))
	{
		std::cerr << "ERROR: Failed to load images/CHARACTERSELECTION.png" << std::endl;
	}
	CSELECTION.setTexture(cselection);
	sf::Vector2u imagesize = cselection.getSize();
	if (imagesize.x > 0 && imagesize.y > 0)
	{
		CSELECTION.setScale(800.f / imagesize.x, 600.f / imagesize.y);
	}

	const char* stripNames[6] = {
		"images/ch1_strip1.png",
		"images/ch2_strip1.png",
		"images/ch3_strip1.png",
		"images/ch4_strip1.png",
		"images/ch5_strip1.png",
		"images/ch6_strip1.png"
	};

	for (int i = 0; i < 6; ++i)
	{
		int w = frameWFor(i);
		int h = frameHFor(i);
		if (stripTex[i].loadFromFile(stripNames[i]))
		{
			stripLoaded[i] = true;
			if (w == 0)
			{
				sf::Vector2u size = stripTex[i].getSize();
				w = static_cast<int>(size.x) / FRAME_COUNT;
				h = static_cast<int>(size.y);
			}
			strips[i].setTexture(stripTex[i]);
			strips[i].setTextureRect(sf::IntRect(0, 0, w, h));
			strips[i].setPosition(cardX[i % 3], cardY);
		}
		else
		{
			stripLoaded[i] = false;
			std::cerr << "NOTE: Missing " << stripNames[i]
			          << " - showing placeholder on the selection screen." << std::endl;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		const char* names[3] = { "images/NEXT1.png", "images/NEXT2.png", "images/NEXT3.png" };
		if (buttonTex[i].loadFromFile(names[i]))
		{
			selectButtons[i].setTexture(buttonTex[i]);
			selectButtons[i].setPosition(buttonX[i], buttonY);
			selectButtons[i].setScale(0.2f, 0.2f);
		}
	}

	if (nextPageTex.loadFromFile("images/NEXTBUTTON.png"))
	{
		NEXT_PAGE.setTexture(nextPageTex);
		NEXT_PAGE.setPosition(610.f, 525.f);
		NEXT_PAGE.setScale(0.12f, 0.12f);

		PREV_PAGE.setTexture(nextPageTex);
		PREV_PAGE.setPosition(170.f, 525.f);
		PREV_PAGE.setScale(-0.12f, 0.12f); // flipped: acts as a back-to-page-1 arrow
	}

	font.loadFromFile("C:/Windows/Fonts/arial.ttf");
}

void chselection::draw(sf::RenderWindow& window)
{
	window.draw(CSELECTION);

	int start = page * 3;
	for (int i = start; i < start + 3; ++i)
	{
		int slot = i % 3;
		if (stripLoaded[i])
		{
			window.draw(strips[i]);
		}
		else
		{
			// Placeholder box for characters you haven't added art for yet.
			sf::RectangleShape box(sf::Vector2f(160.f, 200.f));
			box.setPosition(cardX[slot] - 80.f, cardY);
			box.setFillColor(sf::Color(60, 60, 70, 200));
			box.setOutlineColor(sf::Color(180, 180, 180));
			box.setOutlineThickness(2.f);
			window.draw(box);

			if (font.getInfo().family.size() > 0)
			{
				sf::Text label("CHAR " + std::to_string(i + 1), font, 20);
				label.setFillColor(sf::Color(220, 220, 220));
				sf::FloatRect b = label.getLocalBounds();
				label.setOrigin(b.width / 2.f, b.height / 2.f);
				label.setPosition(cardX[slot], cardY + 100.f);
				window.draw(label);
			}
		}
		window.draw(selectButtons[slot]);
	}

	if (page == 0)
		window.draw(NEXT_PAGE);
	else
		window.draw(PREV_PAGE);

	// highlight the currently selected character if it's on this page
	int sel = selected;
	if (sel >= 0 && sel / 3 == page && stripLoaded[sel])
	{
		sf::FloatRect b = strips[sel].getGlobalBounds();
		sf::RectangleShape outline(sf::Vector2f(b.width + 10.f, b.height + 10.f));
		outline.setPosition(b.left - 5.f, b.top - 5.f);
		outline.setFillColor(sf::Color::Transparent);
		outline.setOutlineColor(sf::Color(255, 220, 60));
		outline.setOutlineThickness(5.f);
		window.draw(outline);
	}
}

void chselection::update(float dt)
{
	animTimer += dt;

	if (animTimer >= frameDuration)
	{
		animTimer = 0.f;
		currentFrame = (currentFrame + 1) % FRAME_COUNT;

		for (int i = 0; i < 6; ++i)
		{
			if (stripLoaded[i])
			{
				sf::Vector2u size = stripTex[i].getSize();
				int w = static_cast<int>(size.x) / FRAME_COUNT;
				int h = static_cast<int>(size.y);
				strips[i].setTextureRect(
					sf::IntRect(currentFrame * w, 0, w, h)
				);
			}
		}
	}
}

int chselection::handleClick(sf::Vector2f mousePos)
{
	int start = page * 3;
	for (int slot = 0; slot < 3; ++slot)
	{
		if (selectButtons[slot].getGlobalBounds().contains(mousePos))
			return start + slot;
	}
	return -1; // nothing hit
}

bool chselection::isNextPageClicked(sf::Vector2f mousePos)
{
	return NEXT_PAGE.getGlobalBounds().contains(mousePos);
}

bool chselection::isPrevPageClicked(sf::Vector2f mousePos)
{
	return PREV_PAGE.getGlobalBounds().contains(mousePos);
}
