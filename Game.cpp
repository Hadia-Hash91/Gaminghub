#include "Game.h"
#include <string>
Game::Game()
{
    window.create(sf::VideoMode(800, 600), "Gaming Hub");

    font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(20.f, 20.f);   
    
    livesText.setFont(font);              // NEW
    livesText.setCharacterSize(24);       // NEW
    livesText.setFillColor(sf::Color::Black); // NEW
    livesText.setPosition(20.f, 50.f);    // top-left corner
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
            if (gameSelectionScreen.iswatermelonclicked(mousepos))
            {
                state = GameState::Watermelon;
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
        scoreText.setString("Score: " + std::to_string(score));
        livesText.setString("Lives: " + std::to_string(player.getLives()));

        if (state == GameState::MainpageScreen)
        {
            menu.updateMainStartHover(mousePos);
            menu.updateSettingsHover(mousePos);
            menu.updateLeaderboardHover(mousePos);
        }

        if (state == GameState::GameSelectionScreen)
        {
            gameSelectionScreen.updatewatermelonHover(mousePos);
            gameSelectionScreen.updatechessHover(mousePos);
            gameSelectionScreen.updatehowtoplay1Hover(mousePos);
        }

        if (state == GameState::Watermelon)
        {
            player.update();

            float dt = gameClock.restart().asSeconds();

            if (spawnClock.getElapsedTime().asSeconds() > 1.5f)
            {
                spawnClock.restart();

                // find a free (inactive) slot in the array
                for (int i = 0; i < MAX_WATERMELONS; i++)
                {
                    if (!watermelons[i].isActive())
                    {
                        float randomX = 100.f + (rand() % 600);
                        watermelons[i].spawn(randomX);
                        break; // stop looking once we've used one slot
                    }
                }
            }

            for (int i = 0; i < MAX_WATERMELONS; i++)
            {
                if (watermelons[i].isActive())
                    
                    watermelons[i].update(dt);
            }

            if (player.isKnifeActive())
            {
                for (int i = 0; i < MAX_WATERMELONS; i++)
                {
                    if (watermelons[i].isActive() && !watermelons[i].getIsCut())   // NEW: only if not already cut
                    {
                        if (watermelons[i].getBounds().intersects(player.getKnifeBounds()))
                        {
                            watermelons[i].cut();
                            score += 10;
                        }
                    }
                }
            }
            for (int i = 0; i < MAX_WATERMELONS; i++)
            {
                if (watermelons[i].isActive() && !watermelons[i].getIsCut())
                {
                    if (watermelons[i].getBounds().intersects(player.getBounds()))
                    {
                        player.loseLife();
                        watermelons[i].cut();
                    }
                }
            }
        }

    }


    void Game::render()
    {
        window.clear(sf::Color::White);   

        if (state == GameState::MenuScreen)
            menu.drawMenu(window);
        else if (state == GameState::MainpageScreen)  
            menu.drawMainpage(window);
		else if (state == GameState::settingsScreen)
            settingsScreen.drawSettings(window);
		else if (state == GameState::GameSelectionScreen)
			gameSelectionScreen.drawGameSelection(window);
		else if (state == GameState::LeaderboardScreen)
            leaderboardScreen.drawLeaderboard(window);
        
        else if (state == GameState::Watermelon)
        {
            watermelonGame.drawwatermelon(window);
            player.draw(window);

            for (int i = 0; i < MAX_WATERMELONS; i++)
            {
                if (watermelons[i].isActive())
                    watermelons[i].draw(window);
            }
            window.draw(scoreText);
			window.draw(livesText);
            if (player.getLives() <= 0)
            {
                state = GameState::MainpageScreen;   // or a dedicated GameOver state, your choice
            }
        }
        
        else if (state == GameState::chess)
        {
            
        }
        else if (state == GameState::HowToPlay1)
        {
            watermelonGame.drawwatermelon1(window);

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