#include "TileTetris.h"
#include <ctime>
#include <sstream>

TileTetris::TileTetris(sf::RenderWindow& window)
    : window_(window),
      state_(TetrisState::Menu),
      difficulty_(TetrisDifficulty::Medium),
      current_(TetrominoType::I),
      next_(TetrominoType::I),
      score_(0),
      linesClearedTotal_(0),
      level_(1),
      dropTimer_(0.f),
      dropInterval_(0.8f),
      rng_(static_cast<unsigned>(std::time(nullptr)))
{
    assets_.loadAll(); // ok even if images are missing - fallback colors kick in
    bgLoaded_ = bgTexture_.loadFromFile("IMAGES/tetrisbackground.png");
}

void TileTetris::reset()
{
    state_ = TetrisState::Menu;
    difficulty_ = TetrisDifficulty::Medium;
    exitRequested_ = false;
    board_.reset();
    score_ = 0;
    linesClearedTotal_ = 0;
    level_ = 1;
}

float TileTetris::baseDropIntervalFor(TetrisDifficulty d) const {
    switch (d) {
        case TetrisDifficulty::Easy:   return 0.90f; // slow, forgiving
        case TetrisDifficulty::Medium: return 0.55f;
        case TetrisDifficulty::Hard:   return 0.28f; // fast, unforgiving
    }
    return 0.55f;
}

Tetromino TileTetris::makeRandomPiece() {
    std::uniform_int_distribution<int> dist(0, TETROMINO_COUNT - 1);
    TetrominoType t = static_cast<TetrominoType>(dist(rng_));
    return Tetromino(t);
}

void TileTetris::startGame(TetrisDifficulty diff) {
    difficulty_ = diff;
    board_.reset();
    score_ = 0;
    linesClearedTotal_ = 0;
    level_ = 1;
    dropInterval_ = baseDropIntervalFor(diff);
    dropTimer_ = 0.f;

    current_ = makeRandomPiece();
    next_ = makeRandomPiece();
    state_ = TetrisState::Playing;
}

void TileTetris::updateDropSpeed() {
    // Every 5 lines, the board speeds up a little, on top of the
    // difficulty's base speed. Floors out so it never becomes unplayable-fast.
    level_ = 1 + linesClearedTotal_ / 5;
    float speedFactor = 1.0f - (level_ - 1) * 0.07f;
    if (speedFactor < 0.35f) speedFactor = 0.35f;
    dropInterval_ = baseDropIntervalFor(difficulty_) * speedFactor;
}

void TileTetris::spawnNewPiece() {
    current_ = next_;
    current_.setPosition(BOARD_WIDTH / 2 - 2, 0);
    next_ = makeRandomPiece();

    if (!board_.isValidPosition(current_)) {
        state_ = TetrisState::GameOver;
    }
}

bool TileTetris::tryMove(int dx, int dy) {
    current_.moveBy(dx, dy);
    if (board_.isValidPosition(current_)) {
        return true;
    }
    current_.moveBy(-dx, -dy); // undo
    return false;
}

void TileTetris::tryRotate() {
    current_.rotateClockwise();
    if (board_.isValidPosition(current_)) return;

    // Simple wall-kick attempt: nudge left/right a bit before giving up.
    current_.moveBy(1, 0);
    if (board_.isValidPosition(current_)) return;
    current_.moveBy(-2, 0);
    if (board_.isValidPosition(current_)) return;
    current_.moveBy(1, 0);

    current_.undoRotate(); // couldn't rotate anywhere - cancel it
}

void TileTetris::hardDrop() {
    while (tryMove(0, 1)) {
        score_ += 2; // small bonus for hard-dropping
    }
    lockCurrentPiece();
}

void TileTetris::lockCurrentPiece() {
    board_.lockPiece(current_);
    int cleared = board_.clearFullLines();

    if (cleared > 0) {
        // Classic-style scoring: more lines at once = bigger bonus.
        static const int points[5] = {0, 100, 300, 500, 800};
        score_ += points[cleared] * level_;
        linesClearedTotal_ += cleared;
        updateDropSpeed();
    }

    if (board_.isTopRowBlocked()) {
        state_ = TetrisState::GameOver;
        return;
    }

    spawnNewPiece();
}

void TileTetris::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (state_ == TetrisState::Menu) {
            if (event.key.code == sf::Keyboard::Num1) startGame(TetrisDifficulty::Easy);
            else if (event.key.code == sf::Keyboard::Num2) startGame(TetrisDifficulty::Medium);
            else if (event.key.code == sf::Keyboard::Num3) startGame(TetrisDifficulty::Hard);
            else if (event.key.code == sf::Keyboard::Escape) exitRequested_ = true;
        }
        else if (state_ == TetrisState::Playing) {
            if (event.key.code == sf::Keyboard::Left)       tryMove(-1, 0);
            else if (event.key.code == sf::Keyboard::Right) tryMove(1, 0);
            else if (event.key.code == sf::Keyboard::Down)  { if (tryMove(0, 1)) score_ += 1; }
            else if (event.key.code == sf::Keyboard::Up)    tryRotate();
            else if (event.key.code == sf::Keyboard::Space) hardDrop();
            else if (event.key.code == sf::Keyboard::P)     state_ = TetrisState::Paused;
            else if (event.key.code == sf::Keyboard::Escape) exitRequested_ = true;
        }
        else if (state_ == TetrisState::Paused) {
            if (event.key.code == sf::Keyboard::P) state_ = TetrisState::Playing;
            else if (event.key.code == sf::Keyboard::Escape) exitRequested_ = true;
        }
        else if (state_ == TetrisState::GameOver) {
            if (event.key.code == sf::Keyboard::Enter) state_ = TetrisState::Menu;
            else if (event.key.code == sf::Keyboard::Escape) exitRequested_ = true;
        }
    }
}

void TileTetris::update(float dt) {
    if (state_ != TetrisState::Playing) return;

    dropTimer_ += dt;
    if (dropTimer_ >= dropInterval_) {
        dropTimer_ = 0.f;
        if (!tryMove(0, 1)) {
            lockCurrentPiece();
        }
    }
}

void TileTetris::render() {
    window_.clear(sf::Color(10, 10, 14));

    if (bgLoaded_) {
        sf::Sprite bg(bgTexture_);
        sf::Vector2u size = bgTexture_.getSize();
        bg.setScale(static_cast<float>(WINDOW_WIDTH) / size.x,
                    static_cast<float>(WINDOW_HEIGHT) / size.y);
        window_.draw(bg);
    }

    if (state_ == TetrisState::Menu) {
        renderMenu();
    } else {
        board_.draw(window_, assets_);

        if (state_ == TetrisState::Playing || state_ == TetrisState::Paused) {
            // draw the falling piece on top of the locked board
            auto blocks = current_.getBlocks();
            const sf::Texture* tex = assets_.getTexture(current_.getType());
            for (const auto& b : blocks) {
                if (b.y < 0) continue; // part of piece still above the visible board
                if (tex) {
                    drawSpriteFit(window_, *tex, static_cast<float>(b.x * CELL_SIZE),
                                  static_cast<float>(b.y * CELL_SIZE), CELL_SIZE);
                } else {
                    sf::RectangleShape rect(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
                    rect.setPosition(static_cast<float>(b.x * CELL_SIZE + 1),
                                      static_cast<float>(b.y * CELL_SIZE + 1));
                    rect.setFillColor(fallbackColor(current_.getType()));
                    rect.setOutlineThickness(1.f);
                    rect.setOutlineColor(sf::Color::White);
                    window_.draw(rect);
                }
            }
        }

        renderHUD();
        renderNextPreview();

        if (state_ == TetrisState::Paused) renderPaused();
        if (state_ == TetrisState::GameOver) renderGameOver();
    }
}

void TileTetris::renderMenu() {
    sf::RectangleShape panel(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    panel.setFillColor(sf::Color(15, 15, 20));
    window_.draw(panel);

    if (!assets_.hasFont()) return; // no font added yet - menu is blank but game still works via number keys

    sf::Text title("TILE TETRIS", assets_.getFont(), 40);
    title.setFillColor(sf::Color::White);
    title.setPosition(WINDOW_WIDTH / 2.f - title.getLocalBounds().width / 2.f, 60.f);
    window_.draw(title);

    auto drawOption = [&](const std::string& label, float y) {
        sf::Text t(label, assets_.getFont(), 24);
        t.setFillColor(sf::Color(230, 230, 230));
        t.setPosition(WINDOW_WIDTH / 2.f - t.getLocalBounds().width / 2.f, y);
        window_.draw(t);
    };

    drawOption("Press 1  -  EASY", 220.f);
    drawOption("Press 2  -  MEDIUM", 270.f);
    drawOption("Press 3  -  HARD", 320.f);

    sf::Text controls("Arrows: move / rotate   Space: hard drop   P: pause   Esc: back",
                       assets_.getFont(), 14);
    controls.setFillColor(sf::Color(150, 150, 150));
    controls.setPosition(WINDOW_WIDTH / 2.f - controls.getLocalBounds().width / 2.f, 420.f);
    window_.draw(controls);
}

void TileTetris::renderHUD() {
    int panelX = BOARD_WIDTH * CELL_SIZE;

    sf::RectangleShape panel(sf::Vector2f(SIDE_PANEL_WIDTH, WINDOW_HEIGHT));
    panel.setPosition(static_cast<float>(panelX), 0.f);
    panel.setFillColor(sf::Color(24, 24, 30, 150));
    window_.draw(panel);

    if (!assets_.hasFont()) return;

    std::ostringstream ss;
    ss << "Score\n" << score_ << "\n\nLevel\n" << level_ << "\n\nLines\n" << linesClearedTotal_;

    sf::Text hud(ss.str(), assets_.getFont(), 18);
    hud.setFillColor(sf::Color::White);
    hud.setPosition(panelX + 15.f, 20.f);
    window_.draw(hud);

    sf::Text next("Next:", assets_.getFont(), 16);
    next.setFillColor(sf::Color(200, 200, 200));
    next.setPosition(panelX + 15.f, 260.f);
    window_.draw(next);
}

void TileTetris::renderNextPreview() {
    int panelX = BOARD_WIDTH * CELL_SIZE;
    int previewOriginX = panelX + 20;
    int previewOriginY = 290;

    const sf::Texture* tex = assets_.getTexture(next_.getType());

    // Draw next_ using rotation 0 offsets directly (ignore its board x/y).
    Tetromino preview = next_;
    preview.setPosition(0, 0);
    auto blocks = preview.getBlocks();

    for (const auto& b : blocks) {
        int px = previewOriginX + b.x * (CELL_SIZE - 4);
        int py = previewOriginY + b.y * (CELL_SIZE - 4);

        if (tex) {
            drawSpriteFit(window_, *tex, static_cast<float>(px), static_cast<float>(py),
                          CELL_SIZE - 4);
        } else {
            sf::RectangleShape rect(sf::Vector2f(CELL_SIZE - 8.f, CELL_SIZE - 8.f));
            rect.setPosition(static_cast<float>(px), static_cast<float>(py));
            rect.setFillColor(fallbackColor(next_.getType()));
            window_.draw(rect);
        }
    }
}

void TileTetris::renderPaused() {
    if (!assets_.hasFont()) return;
    sf::Text t("PAUSED - press P to resume", assets_.getFont(), 22);
    t.setFillColor(sf::Color::Yellow);
    t.setPosition(20.f, WINDOW_HEIGHT / 2.f - 20.f);
    window_.draw(t);
}

void TileTetris::renderGameOver() {
    sf::RectangleShape overlay(sf::Vector2f(BOARD_WIDTH * CELL_SIZE, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 170));
    window_.draw(overlay);

    if (!assets_.hasFont()) return;

    sf::Text t("GAME OVER", assets_.getFont(), 32);
    t.setFillColor(sf::Color::White);
    t.setPosition(BOARD_WIDTH * CELL_SIZE / 2.f - t.getLocalBounds().width / 2.f, 180.f);
    window_.draw(t);

    std::ostringstream ss;
    ss << "Score: " << score_;
    sf::Text s(ss.str(), assets_.getFont(), 20);
    s.setFillColor(sf::Color(220, 220, 220));
    s.setPosition(BOARD_WIDTH * CELL_SIZE / 2.f - s.getLocalBounds().width / 2.f, 230.f);
    window_.draw(s);

    sf::Text r("Press ENTER for menu", assets_.getFont(), 16);
    r.setFillColor(sf::Color(180, 180, 180));
    r.setPosition(BOARD_WIDTH * CELL_SIZE / 2.f - r.getLocalBounds().width / 2.f, 270.f);
    window_.draw(r);
}
