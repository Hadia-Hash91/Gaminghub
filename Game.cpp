#include "Game.h"

Game::Game()
{
    window.create(
        sf::VideoMode(800, 600),
        "Gaming Hub"
    );
	

}
void Game::processEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        sf::Vector2f mousepos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // ---------- Start button (MenuScreen) ----------
        if (event.type == sf::Event::MouseButtonPressed &&
            menu.isStartButtonClicked(mousepos) &&
            state == GameState::MenuScreen)
        {
            menu.ispressed();
            isPressed = true;
        }

        if (event.type == sf::Event::MouseButtonReleased && isPressed)
        {
            menu.isreleased();
            isPressed = false;
            state = GameState::MainpageScreen;   // <-- go to mainpage, not "Playing"
        }

        // ---------- Mainpage buttons ----------
        if (event.type == sf::Event::MouseButtonPressed && state == GameState::MainpageScreen)
        {
            if (menu.isMainStartClicked(mousepos))
            {
                state = GameState::GameSelectionScreen;


            }

            if (menu.isSettingsClicked(mousepos))
            {
                // handle "Settings" click
                state = GameState::settingsScreen;
            }

            if (menu.isLeaderboardClicked(mousepos))
            {
				state = GameState::LeaderboardScreen;
            }
            
        }
        if (event.type == sf::Event::MouseButtonPressed && state == GameState::GameSelectionScreen)
        {
            if (gameSelectionScreen.ispurpleplaceclicked(mousepos))
            {
                state = GameState::PurplePlacePlaying;   
            }
           
        }
        if (event.type == sf::Event::MouseButtonPressed && state == GameState::GameSelectionScreen)
        {
            if (gameSelectionScreen.ischessclicked(mousepos))
            {
                state = GameState::chess;
            }

        }
        if (event.type == sf::Event::MouseButtonPressed && state == GameState::GameSelectionScreen)
        {
            if (gameSelectionScreen.ishowtoplay1clicked(mousepos))
            {
                state = GameState::HowToPlay1;
            }
        }
       
    }
}


    void Game::update()
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (state == GameState::MainpageScreen)
        {
            menu.updateMainStartHover(mousePos);
            menu.updateSettingsHover(mousePos);
            menu.updateLeaderboardHover(mousePos);
        }

        if (state == GameState::GameSelectionScreen)
        {
            gameSelectionScreen.updatePurplePlaceHover(mousePos);
            gameSelectionScreen.updatechessHover(mousePos);
            gameSelectionScreen.updatehowtoplay1Hover(mousePos);
        }
    }


    void Game::render()
    {
        window.clear(sf::Color::White);   // also fixing this, see note below

        if (state == GameState::MenuScreen)
            menu.drawMenu(window);
        else if (state == GameState::MainpageScreen)   // <-- fixed to match!
            menu.drawMainpage(window);
		else if (state == GameState::settingsScreen)
            settingsScreen.drawSettings(window);
		else if (state == GameState::GameSelectionScreen)
			gameSelectionScreen.drawGameSelection(window);
		else if (state == GameState::LeaderboardScreen)
            leaderboardScreen.drawLeaderboard(window);
        else if (state == GameState::PurplePlacePlaying)
        {
            // eventually: purplePlaceGame.draw(window);
            // for now, maybe just a placeholder color/text to confirm it switches
        }
        else if (state == GameState::chess)
        {
            // eventually: purplePlaceGame.draw(window);
            // for now, maybe just a placeholder color/text to confirm it switches
        }
        else if (state == GameState::HowToPlay1)
        {
            // eventually: purplePlaceGame.draw(window);
            // for now, maybe just a placeholder color/text to confirm it switches
        }

		
        window.display();
    }

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();

        update();

        render();
    }
}