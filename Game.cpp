#include "Game.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

Game::Game()
    : window(sf::VideoMode(800, 600), "Gaming Hub"),
    tetrisGame(window),
    laneRunnerGame(window)
{
    srand(static_cast<unsigned>(time(nullptr)));

    font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2.f);
    scoreText.setPosition(20.f, 20.f);
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);
    livesText.setOutlineColor(sf::Color::Black);
    livesText.setOutlineThickness(2.f);
    livesText.setPosition(20.f, 50.f);

    backbutton.loadFromFile("IMAGES/back.png");
    BACKBUTTON.setTexture(backbutton);
    BACKBUTTON.setScale(0.2f, 0.2f);
    BACKBUTTON.setPosition(10.f, 10.f);
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

        // ---------- Hitbox toggle (works in any state) ----------
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H)
        {
            showHitboxes = !showHitboxes;
        }

        if (state == GameState::Tetris)
        {
            tetrisGame.handleEvent(event);
        }

        if (state == GameState::LaneRunner)
        {
            laneRunnerGame.handleEvent(event);
        }

        sf::Vector2f mousepos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (state == GameState::settingsScreen)
        {
            settingsScreen.handleEvent(event, mousepos);
        }

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
                leaderboardScreen.reload();
                state = GameState::LeaderboardScreen;
            }
            if (menu.isExitClicked(mousepos))
            {
                window.close();
            }

        }
        if (event.type == sf::Event::MouseButtonPressed && state == GameState::GameSelectionScreen)
        {
            if (gameSelectionScreen.iswatermelonclicked(mousepos))
            {
                resetGame();
                state = GameState::Watermelon;
            }

        }
        if (event.type == sf::Event::MouseButtonPressed && state == GameState::GameSelectionScreen)
        {
            if (gameSelectionScreen.isTetrisGameBoxClicked(mousepos))
            {
                tetrisGame.reset();
                state = GameState::Tetris;
            }

        }
        if (event.type == sf::Event::MouseButtonPressed && state == GameState::GameSelectionScreen)
        {
            if (gameSelectionScreen.ishowtoplay1clicked(mousepos))
            {
                state = GameState::HowToPlay1;
            }
        }
        if (event.type == sf::Event::MouseButtonPressed && state == GameState::GameSelectionScreen)
        {
            if (gameSelectionScreen.isLaneRunnerClicked(mousepos))
            {
                laneRunnerGame.reset();
                laneRunnerGame.setCharacter(selectedCharacter);
                laneRunnerGame.setPlayerName(settingsScreen.getPlayerName());
                laneRunnerGame.setDifficulty(settingsScreen.getDifficulty());
                state = GameState::LaneRunner;
            }
        }
        if (event.type == sf::Event::MouseButtonPressed && state == GameState::MainpageScreen)
        {
            if (menu.ischaracterselectionClicked(mousepos))
            {
                state = GameState::Characterselection;
            }
        }
        if (event.type == sf::Event::MouseButtonPressed && state == GameState::Characterselection)
        {
            if (chsel.isNextPageClicked(mousepos))
                chsel.nextPage();

            if (chsel.isPrevPageClicked(mousepos))
                chsel.prevPage();

            int choice = chsel.handleClick(mousepos);
            if (choice != -1)
            {
                selectedCharacter = choice;
                player.setCharacter(choice);
                chsel.setSelected(choice);
                state = GameState::GameSelectionScreen;
            }
        }
        if (event.type == sf::Event::MouseButtonPressed && state == GameState::GameOver)
        {
            resetGame();
            state = GameState::MainpageScreen;
        }
        if (event.type == sf::Event::MouseButtonPressed &&
            BACKBUTTON.getGlobalBounds().contains(mousepos))
        {
            switch (state)
            {
            case GameState::settingsScreen:
            case GameState::GameSelectionScreen:
            case GameState::LeaderboardScreen:
            case GameState::Characterselection:
                state = GameState::MainpageScreen;
                break;

            case GameState::Watermelon:
            case GameState::HowToPlay1:
            case GameState::Tetris:
            case GameState::LaneRunner:
                tetrisGame.clearExitRequested();
                laneRunnerGame.clearExitRequested();
                state = GameState::GameSelectionScreen;
                break;

            default:
                break; // no back action on MenuScreen / MainpageScreen
            }
        }

    }
}


void Game::update()
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    scoreText.setString("Score: " + std::to_string(score));
    livesText.setString("Lives: " + std::to_string(player.getLives()));
    float dt = gameClock.restart().asSeconds();
    if (dt > 0.05f) dt = 0.05f;   // clamp frame spikes
    if (state == GameState::MainpageScreen)
    {
        menu.updateMainStartHover(mousePos);
        menu.updateSettingsHover(mousePos);
        menu.updateLeaderboardHover(mousePos);
        menu.updatecharacterselectionHover(mousePos);
        menu.updateExitHover(mousePos);

    }

    if (state == GameState::GameSelectionScreen)
    {
        gameSelectionScreen.updatewatermelonHover(mousePos);
        gameSelectionScreen.updateTetrisGameBoxHover(mousePos);
        gameSelectionScreen.updatehowtoplay1Hover(mousePos);
        gameSelectionScreen.updateLaneRunnerHover(mousePos);
    }

    if (state == GameState::Characterselection)
    {

        chsel.update(dt);
    }

    if (state == GameState::Tetris)
    {
        tetrisGame.update(dt);
        if (tetrisGame.exitRequested())
        {
            tetrisGame.clearExitRequested();
            state = GameState::GameSelectionScreen;
        }
    }

    if (state == GameState::LaneRunner)
    {
        laneRunnerGame.update(dt);
        if (laneRunnerGame.exitRequested())
        {
            laneRunnerGame.clearExitRequested();
            state = GameState::GameSelectionScreen;
        }
    }

    if (state == GameState::Watermelon)
    {
        player.update();

        float spawnInterval = std::max(0.5f, 1.5f - score * 0.004f);
        float fallSpeed = std::min(420.f, 140.f + score * 0.9f);

        if (spawnClock.getElapsedTime().asSeconds() > spawnInterval)
        {
            spawnClock.restart();

            // find a free (inactive) slot in the array
            for (int i = 0; i < MAX_WATERMELONS; i++)
            {
                if (!watermelons[i].isActive())
                {
                    float randomX = 100.f + (rand() % 600);
                    watermelons[i].setFallSpeed(fallSpeed);
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
                if (watermelons[i].isActive() && !watermelons[i].getIsCut())
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

        if (player.getLives() <= 0)
            state = GameState::GameOver;
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
    else if (state == GameState::Characterselection)
    {
        chsel.draw(window);
    }
    else if (state == GameState::Watermelon)
    {
        watermelonGame.drawwatermelon(window);
        player.draw(window);

        for (int i = 0; i < MAX_WATERMELONS; i++)
        {
            if (watermelons[i].isActive())
                watermelons[i].draw(window);
        }

        // ---------- Hitboxes: only drawn when H has toggled them on ----------
        if (showHitboxes)
        {
            drawHitbox(window, player.getBounds(), sf::Color::Blue);

            if (player.isKnifeActive())
                drawHitbox(window, player.getKnifeBounds(), sf::Color::Red);

            for (int i = 0; i < MAX_WATERMELONS; i++)
            {
                if (watermelons[i].isActive())
                    drawHitbox(window, watermelons[i].getBounds(), sf::Color::Yellow);
            }
        }

        window.draw(scoreText);
        window.draw(livesText);
    }
    else if (state == GameState::GameOver)
    {
        window.clear(sf::Color(40, 40, 50));

        sf::Text gameOverText("GAME OVER", font, 64);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setOutlineColor(sf::Color::Black);
        gameOverText.setOutlineThickness(3.f);
        gameOverText.setOrigin(gameOverText.getLocalBounds().width / 2.f,
            gameOverText.getLocalBounds().height / 2.f);
        gameOverText.setPosition(400.f, 200.f);

        sf::Text finalScoreText("Final Score: " + std::to_string(score), font, 32);
        finalScoreText.setFillColor(sf::Color::White);
        finalScoreText.setOrigin(finalScoreText.getLocalBounds().width / 2.f,
            finalScoreText.getLocalBounds().height / 2.f);
        finalScoreText.setPosition(400.f, 300.f);

        sf::Text promptText("Click anywhere to return to Main Menu", font, 20);
        promptText.setFillColor(sf::Color(200, 200, 200));
        promptText.setOrigin(promptText.getLocalBounds().width / 2.f,
            promptText.getLocalBounds().height / 2.f);
        promptText.setPosition(400.f, 380.f);

        window.draw(gameOverText);
        window.draw(finalScoreText);
        window.draw(promptText);
    }
    else if (state == GameState::Tetris)
    {
        // Fit the Tetris board (520x640) into the hub window (800x600)
        // while keeping its aspect ratio, centered.
        float scale = std::min(800.f / WINDOW_WIDTH, 600.f / WINDOW_HEIGHT);
        float viewW = (WINDOW_WIDTH * scale) / 800.f;
        float viewH = (WINDOW_HEIGHT * scale) / 600.f;

        sf::View tetrisView(sf::FloatRect(0.f, 0.f,
            static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)));
        tetrisView.setViewport(sf::FloatRect((1.f - viewW) / 2.f, (1.f - viewH) / 2.f, viewW, viewH));

        window.setView(tetrisView);
        tetrisGame.render();
        window.setView(window.getDefaultView());
    }



    else if (state == GameState::LaneRunner)
    {
        laneRunnerGame.render();
    }
    else if (state == GameState::HowToPlay1)
    {
        watermelonGame.drawwatermelon1(window);

    }

    if (state != GameState::MenuScreen && state != GameState::MainpageScreen && state != GameState::GameOver)
    {
        window.draw(BACKBUTTON);
    }

    window.display();
}

void Game::resetGame()
{
    score = 0;
    player.setCharacter(selectedCharacter >= 0 ? selectedCharacter : 0);
    player.resetLives();
    player.resetWeapon();

    for (int i = 0; i < MAX_WATERMELONS; i++)
    {
        if (watermelons[i].isActive())
            watermelons[i].deactivate();
    }

    spawnClock.restart();
    gameClock.restart();
}

void Game::drawHitbox(sf::RenderWindow& window, const sf::FloatRect& rect, const sf::Color& color, float padding)
{
    // padding > 0 draws the outline bigger than the real hitbox (easier to see).
    // padding < 0 draws it smaller. padding = 0 (default) matches the real bounds exactly.
    sf::RectangleShape box(sf::Vector2f(rect.width + padding * 2.f, rect.height + padding * 2.f));
    box.setPosition(rect.left - padding, rect.top - padding);
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(color);
    box.setOutlineThickness(2.f);
    window.draw(box);
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