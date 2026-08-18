#include "LaneRunner.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>

namespace {
    constexpr float LANE_Y[3] = { 600.f * 0.35f, 600.f * 0.55f, 600.f * 0.75f };
    constexpr float GROUND_Y = 600.f * 0.80f;
    constexpr float PLAYER_X = 160.f;
    constexpr float GRAVITY = 2200.f;
    constexpr float JUMP_VELOCITY = -820.f;
    constexpr int COINS_PER_LEVEL = 25;
    constexpr int TOTAL_LEVELS = 5;

    // feet sit on the track line, which is 40 px below LANE_Y
    constexpr float PLAYER_STAND_H = 100.f;
    constexpr float PLAYER_SLIDE_H = 46.f;
    constexpr float HURDLE_H = 46.f;     // low obstacle: jump over it
    constexpr float BEAM_BOTTOM = 52.f;  // tall obstacle: slide under it
    constexpr float BEAM_H = 90.f;
    constexpr float BEAM_VISUAL_H = BEAM_H + BEAM_BOTTOM; // sprite stretches to the ground

    // watermelon power-up
    constexpr float INVINCIBLE_DURATION = 5.f;
    constexpr int WATERMELON_SPAWN_CHANCE = 4; // percent, per spawn tick

    // bee hazard - flies at chest height, bobs up/down, ALWAYS fatal on touch
    constexpr float BEE_BASE_OFFSET = 70.f;  // height above the track line
    constexpr float BEE_BOB_AMPLITUDE = 22.f;
    constexpr float BEE_BOB_SPEED = 3.2f;
    constexpr float BEE_SIZE = 65.f;
    constexpr int BEE_SPAWN_CHANCE = 6; // percent, per spawn tick
}

LaneRunner::LaneRunner(sf::RenderWindow& window)
    : window_(window)
{
    srand(static_cast<unsigned>(time(nullptr)));
    loadAssets();
    player_.init();
    resetGame();
}

float LaneRunner::randRange(float lo, float hi) {
    return lo + static_cast<float>(rand()) / RAND_MAX * (hi - lo);
}

int LaneRunner::randInt(int lo, int hi) { return lo + rand() % (hi - lo + 1); }

// ---- Player ---------------------------------------------------------------
void LaneRunner::Player::init() {
    body.setSize(sf::Vector2f(46.f, 70.f));
    body.setFillColor(sf::Color(240, 90, 60));
    body.setOrigin(23.f, 70.f);
    head.setRadius(16.f);
    head.setFillColor(sf::Color(255, 210, 160));
    head.setOrigin(16.f, 16.f);
}

void LaneRunner::Player::moveLane(int dir) {
    int next = lane + dir;
    if (next < 0) next = 0;
    if (next > 2) next = 2;
    lane = next;
    targetY = LANE_Y[lane];
}

bool LaneRunner::Player::requestJump() {
    if (!jumping && !sliding) {
        jumping = true;
        return true;
    }
    return false;
}

void LaneRunner::Player::slide() {
    if (!jumping && !sliding) {
        sliding = true;
        slideTimer = 0.55f;
    }
}

void LaneRunner::Player::update(float dt) {
    float step = std::min(1.f, dt * 12.f);
    y += (targetY - y) * step;
    if (sliding) {
        slideTimer -= dt;
        if (slideTimer <= 0.f) sliding = false;
    }
}

sf::FloatRect LaneRunner::Player::getHitbox() const {
    float h = sliding ? PLAYER_SLIDE_H : PLAYER_STAND_H;
    float w = 40.f;
    float bottom = y + 40.f - jumpArc;   // feet on the track line
    return sf::FloatRect(PLAYER_X - w / 2.f, bottom - h, w, h);
}

// ---- setup ----------------------------------------------------------------
void LaneRunner::loadAssets() {
    if (!font_.loadFromFile("assets/font.ttf")) {
        font_.loadFromFile("C:/Windows/Fonts/arial.ttf");
    }

    hasCoin_ = texCoin_.loadFromFile("IMAGES/coin.png");
    hasTall_ = texTall_.loadFromFile("IMAGES/obstacle_tall.png");
    hasLow_ = texLow_.loadFromFile("IMAGES/obstacle_low.png");
    hasBg_ = texBg_.loadFromFile("IMAGES/background.png");

    // Power-up + hazard art. Both fall back to drawn shapes if missing, so
    // the game is fully playable/testable before you drop the real art in.
    hasWatermelon_ = texWatermelon_.loadFromFile("IMAGES/watermelonpiece.png");

    hasBee_ = texBee_.loadFromFile("IMAGES/bee_strip.png");
    if (hasBee_) {
        sf::Vector2u sz = texBee_.getSize();
        beeFrameW_ = static_cast<int>(sz.x) / BEE_FRAME_COUNT;
        beeFrameH_ = static_cast<int>(sz.y);
    }

    // Default player art (single image) until setCharacter picks a strip.
    hasIdle_ = setStripTexture(texIdle_, "IMAGES/LaneRunner/player.png",
        idleFrameW_, idleFrameH_, idleContentH_,
        idleContentBottom_, 1);
}

// Finds the vertical span of non-transparent pixels so the sprite can be
// scaled by its real visible height instead of the (mostly padding) cell.
void LaneRunner::measureStrip(const sf::Texture& tex, int& contentTop, int& contentBottom) {
    sf::Vector2u sz = tex.getSize();
    int w = static_cast<int>(sz.x);
    int h = static_cast<int>(sz.y);
    int minY = h, maxY = -1;
    if (w > 0 && h > 0) {
        sf::Image img = tex.copyToImage();
        const sf::Uint8* px = img.getPixelsPtr();
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                if (px[(y * w + x) * 4 + 3] > 16) {
                    if (y < minY) minY = y;
                    if (y > maxY) maxY = y;
                }
            }
        }
    }
    if (maxY < minY) { contentTop = 0; contentBottom = h - 1; }
    else { contentTop = minY; contentBottom = maxY; }
}

// Loads a sprite strip, works out its frame cell size (width / frame count)
// and measures the real visible height - the same approach the shooter uses.
bool LaneRunner::setStripTexture(sf::Texture& tex, const std::string& path,
    int& frameW, int& frameH, float& contentH,
    float& contentBottom, int frameCount) {
    if (!tex.loadFromFile(path)) return false;
    sf::Vector2u sz = tex.getSize();
    frameW = frameCount > 0 ? static_cast<int>(sz.x) / frameCount : static_cast<int>(sz.x);
    frameH = static_cast<int>(sz.y);
    int top = 0, bottom = frameH - 1;
    measureStrip(tex, top, bottom);
    contentH = static_cast<float>(bottom - top + 1);
    contentBottom = static_cast<float>(bottom);
    return frameW > 0 && frameH > 0 && contentH > 0.f;
}

void LaneRunner::setCharacter(int index) {
    if (index < 0 || index > 5) index = 0;

    static const char* idleNames[6] = {
        "IMAGES/idle_strip.png",
        "IMAGES/ch2_idle_strip.png",
        "IMAGES/ch3_idle_strip1.png",
        "IMAGES/ch4_idle_strip.png",
        "IMAGES/ch5_idle_strip.png",
        "IMAGES/ch6_idle_strip.png"
    };
    static const char* runNames[6] = {
        "IMAGES/ch1_run_strip.png",
        "IMAGES/ch2_run_strip1.png",
        "IMAGES/ch3_run_strip1.png",
        "IMAGES/ch4_run_strip1.png",
        "IMAGES/ch5_run_strip1.png",
        "IMAGES/ch6_run_strip1.png"
    };

    // Idle strip: every character has its own; missing art falls back to ch1.
    hasIdle_ = setStripTexture(texIdle_, idleNames[index],
        idleFrameW_, idleFrameH_, idleContentH_,
        idleContentBottom_, idleFrameCount_);
    if (!hasIdle_) {
        index = 0;
        hasIdle_ = setStripTexture(texIdle_, idleNames[0],
            idleFrameW_, idleFrameH_, idleContentH_,
            idleContentBottom_, idleFrameCount_);
    }

    // Run strip: if a character has no run art yet, animate its own idle
    // strip so the picked character never shows another character's sprite.
    hasRun_ = setStripTexture(texRun_, runNames[index],
        runFrameW_, runFrameH_, runContentH_,
        runContentBottom_, runFrameCount_);
    if (!hasRun_) {
        hasRun_ = setStripTexture(texRun_, idleNames[index],
            runFrameW_, runFrameH_, runContentH_,
            runContentBottom_, runFrameCount_);
    }

    animFrame_ = 0;
    animTimer_ = 0.f;
}

void LaneRunner::setPlayerName(const std::string& name) {
    playerName_ = name.empty() ? "PLAYER" : name;
}

void LaneRunner::setDifficulty(int level) {
    if (level < 0) level = 0;
    if (level > 2) level = 2;
    difficulty_ = level;
    speedFactor_ = (level == 0) ? 0.85f : (level == 2) ? 1.2f : 1.0f;
}

void LaneRunner::reset() {
    state_ = GameState::Menu;
    exitRequested_ = false;
    resetGame();
}

void LaneRunner::resetGame() {
    level_ = 1;
    totalCoins_ = 0;
    score_ = 0;
    savedThisRun_ = false;
    nameEntry_ = playerName_;
    player_.invincible = false;
    player_.invincibleTimer = 0.f;
    setupLevel();
}

void LaneRunner::saveScore() {
    if (savedThisRun_) return;
    savedThisRun_ = true;

    std::time_t now = std::time(nullptr);
    std::tm tm;
    localtime_s(&tm, &now);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);

    std::ofstream file("data/leaderboard.csv", std::ios::app);
    if (!file.is_open()) return;
    std::string name = nameEntry_.empty() ? playerName_ : nameEntry_;
    file << "LaneRunner," << name << "," << score_ << "," << totalCoins_ << ","
        << level_ << "," << buf << "\n";
}

void LaneRunner::setupLevel() {
    coinsThisLevel_ = 0;
    distance_ = 0.f;
    entities_.clear();
    player_.lane = 1;
    player_.y = LANE_Y[1];
    player_.targetY = LANE_Y[1];
    player_.jumping = false;
    player_.sliding = false;
    realJumpHeight_ = 0.f;
    realJumpVel_ = 0.f;
    scrollSpeed_ = (320.f + (level_ - 1) * 55.f) * speedFactor_;
    spawnInterval_ = std::max(0.45f, 1.15f - (level_ - 1) * 0.14f);
    spawnTimer_ = 0.6f;
    animFrame_ = 0;
    animTimer_ = 0.f;
    beeAnimFrame_ = 0;
    beeAnimTimer_ = 0.f;
}

// ---- events ---------------------------------------------------------------
void LaneRunner::handleEvent(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed &&
        event.type != sf::Event::TextEntered) return;

    if (state_ == GameState::Menu) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter)
                state_ = GameState::Playing;
            else if (event.key.code == sf::Keyboard::Escape) exitRequested_ = true;
        }
    }
    else if (state_ == GameState::Playing) {
        if (event.type != sf::Event::KeyPressed) return;
        if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
            player_.moveLane(-1);
        else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
            player_.moveLane(1);
        else if (event.key.code == sf::Keyboard::Space) {
            if (player_.requestJump()) {
                realJumpVel_ = JUMP_VELOCITY;
                realJumpHeight_ = 0.f;
            }
        }
        else if (event.key.code == sf::Keyboard::LShift)
            player_.slide();
        else if (event.key.code == sf::Keyboard::P)
            state_ = GameState::Paused;
        else if (event.key.code == sf::Keyboard::Escape) exitRequested_ = true;
    }
    else if (state_ == GameState::Paused) {
        if (event.type != sf::Event::KeyPressed) return;
        if (event.key.code == sf::Keyboard::P) state_ = GameState::Playing;
        else if (event.key.code == sf::Keyboard::Escape) exitRequested_ = true;
    }
    else if (state_ == GameState::LevelClear) {
        if (event.type != sf::Event::KeyPressed) return;
        if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter) {
            level_++;
            if (level_ > TOTAL_LEVELS) {
                saveScore();
                state_ = GameState::Win;
            }
            else { setupLevel(); state_ = GameState::Playing; }
        }
    }
    else if (state_ == GameState::GameOver) {
        // type your name (shown on the leaderboard), then retry
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode >= 32 && event.text.unicode < 127) {
                char ch = static_cast<char>(event.text.unicode);
                if (ch != ',' && nameEntry_.size() < 12) nameEntry_ += ch;
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Backspace && !nameEntry_.empty())
                nameEntry_.pop_back();
            else if (event.key.code == sf::Keyboard::Space ||
                event.key.code == sf::Keyboard::Enter) {
                saveScore();
                resetGame();
                state_ = GameState::Playing;
            }
        }
    }
    else if (state_ == GameState::Win) {
        if (event.type != sf::Event::KeyPressed) return;
        if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter) {
            resetGame();
            state_ = GameState::Menu;
        }
    }
}

// ---- update ---------------------------------------------------------------
void LaneRunner::update(float dt) {
    fxClock_ += dt;

    animTimer_ += dt;
    if (animTimer_ >= animFrameDuration_) {
        animTimer_ -= animFrameDuration_;
        int count = (state_ == GameState::Playing && hasRun_) ? runFrameCount_ : idleFrameCount_;
        if (count > 0) animFrame_ = (animFrame_ + 1) % count;
    }

    beeAnimTimer_ += dt;
    if (beeAnimTimer_ >= beeAnimFrameDuration_) {
        beeAnimTimer_ -= beeAnimFrameDuration_;
        beeAnimFrame_ = (beeAnimFrame_ + 1) % BEE_FRAME_COUNT;
    }

    if (state_ != GameState::Playing) return;
    updatePlaying(dt);
}

void LaneRunner::updatePlaying(float dt) {
    if (player_.jumping) {
        realJumpVel_ += GRAVITY * dt;
        realJumpHeight_ -= realJumpVel_ * dt;
        if (realJumpHeight_ <= 0.f) {
            realJumpHeight_ = 0.f;
            realJumpVel_ = 0.f;
            player_.jumping = false;
        }
    }
    player_.jumpArc = realJumpHeight_;

    player_.update(dt);

    // invincibility countdown (watermelon power-up)
    if (player_.invincible) {
        player_.invincibleTimer -= dt;
        if (player_.invincibleTimer <= 0.f) {
            player_.invincible = false;
            player_.invincibleTimer = 0.f;
        }
    }

    bgOffset_ += scrollSpeed_ * dt;
    if (bgOffset_ > 64.f) bgOffset_ -= 64.f;

    distance_ += scrollSpeed_ * dt;
    score_ += static_cast<int>(scrollSpeed_ * dt * 0.02f);   // distance points

    spawnTimer_ -= dt;
    if (spawnTimer_ <= 0.f) {
        spawnEntity();
        spawnTimer_ = spawnInterval_ + randRange(-0.15f, 0.15f);
    }

    for (auto it = entities_.begin(); it != entities_.end();) {
        it->x -= scrollSpeed_ * dt;

        sf::FloatRect entRect = entityRect(*it);
        sf::FloatRect playerRect = player_.getHitbox();

        if (!it->collected && entRect.intersects(playerRect)) {
            if (it->type == EntityType::Coin) {
                it->collected = true;
                coinsThisLevel_++;
                totalCoins_++;
                score_ += 25;
            }
            else if (it->type == EntityType::PowerUp) {
                it->collected = true;
                player_.invincible = true;
                player_.invincibleTimer = INVINCIBLE_DURATION;
            }
            else if (it->type == EntityType::Bee) {
                // ALWAYS fatal - invincibility does NOT protect against the
                // bee, and jumping/sliding doesn't dodge it either. Only a
                // lane change avoids it.
                if (state_ != GameState::GameOver) {
                    state_ = GameState::GameOver;
                    nameEntry_ = playerName_;
                }
            }
            else { // Obstacle
                bool avoided = false;
                if (it->tall && player_.sliding) avoided = true;        // duck under the bar
                if (!it->tall && player_.jumpArc > HURDLE_H + 4.f) avoided = true; // jump the hurdle
                if (!avoided && !player_.invincible && state_ != GameState::GameOver) {
                    state_ = GameState::GameOver;
                    nameEntry_ = playerName_;
                }
            }
        }

        if (it->x < -60.f) {
            it = entities_.erase(it);
        }
        else {
            ++it;
        }
    }

    if (coinsThisLevel_ >= COINS_PER_LEVEL) {
        score_ += 1000; // level-clear bonus
        state_ = GameState::LevelClear;
    }
}

float LaneRunner::trackYFor(int lane) const { return LANE_Y[lane] + 40.f; }

// Bee's vertical position: base offset above the track, bobbing with a sine
// wave. Each bee gets its own phase so a row of bees doesn't bob in lockstep.
float LaneRunner::beeYFor(const Entity& e) const {
    float ty = trackYFor(e.lane);
    return ty - BEE_BASE_OFFSET + std::sin(fxClock_ * BEE_BOB_SPEED + e.phase) * BEE_BOB_AMPLITUDE;
}

sf::FloatRect LaneRunner::entityRect(const Entity& e) const {
    float ty = trackYFor(e.lane);
    if (e.type == EntityType::Coin) {
        float cy = ty - (e.tall ? 110.f : 60.f);
        return sf::FloatRect(e.x - 15.f, cy - 15.f, 30.f, 30.f);
    }
    else if (e.type == EntityType::PowerUp) {
        return sf::FloatRect(e.x - 17.f, ty - 60.f - 17.f, 34.f, 34.f);
    }
    else if (e.type == EntityType::Bee) {
        float by = beeYFor(e);
        return sf::FloatRect(e.x - BEE_SIZE / 2.f, by - BEE_SIZE / 2.f, BEE_SIZE, BEE_SIZE);
    }
    else {
        if (e.tall)
            return sf::FloatRect(e.x - 24.f, ty - BEAM_BOTTOM - BEAM_H, 48.f, BEAM_H);
        else
            return sf::FloatRect(e.x - 28.f, ty - HURDLE_H, 56.f, HURDLE_H);
    }
}

void LaneRunner::spawnEntity() {
    int lane = randInt(0, 2);

    // Bee and watermelon are rare, independent rolls checked before the
    // normal coin/obstacle spawn so they don't crowd each other out.
    if ((rand() % 100) < BEE_SPAWN_CHANCE) {
        Entity e;
        e.type = EntityType::Bee;
        e.lane = lane;
        e.x = 900.f + 60.f;
        e.phase = randRange(0.f, 6.283f);
        entities_.push_back(e);
        return;
    }

    if (!player_.invincible && (rand() % 100) < WATERMELON_SPAWN_CHANCE) {
        Entity e;
        e.type = EntityType::PowerUp;
        e.lane = lane;
        e.x = 900.f + 60.f;
        entities_.push_back(e);
        return;
    }

    bool spawnCoinRow = (rand() % 100) < 55;
    if (spawnCoinRow) {
        int rowLen = randInt(3, 6);
        bool arcHigh = (rand() % 100) < 30;
        for (int i = 0; i < rowLen; ++i) {
            Entity e;
            e.type = EntityType::Coin;
            e.lane = lane;
            e.x = 900.f + 60.f + i * 45.f;
            e.tall = arcHigh;
            entities_.push_back(e);
        }
    }
    else {
        // never block all 3 lanes at the same spot - pick a lane that is free
        int tries = 0;
        while (tries < 6) {
            bool blocked = false;
            for (auto& e : entities_) {
                if (e.type == EntityType::Obstacle && e.lane == lane &&
                    std::fabs(e.x - (900.f + 60.f)) < 80.f) {
                    blocked = true;
                    break;
                }
            }
            if (!blocked) break;
            lane = randInt(0, 2);
            tries++;
        }

        Entity e;
        e.type = EntityType::Obstacle;
        e.lane = lane;
        e.x = 900.f + 60.f;
        e.tall = (rand() % 2 == 0);
        entities_.push_back(e);
    }
}

// ---- render ---------------------------------------------------------------
void LaneRunner::render() {
    sf::Vector2u winSize = window_.getSize();
    float scale = std::min(static_cast<float>(winSize.x) / 900.f,
        static_cast<float>(winSize.y) / 600.f);
    float viewW = (900.f * scale) / static_cast<float>(winSize.x);
    float viewH = (600.f * scale) / static_cast<float>(winSize.y);

    sf::View runnerView(sf::FloatRect(0.f, 0.f, 900.f, 600.f));
    runnerView.setViewport(sf::FloatRect((1.f - viewW) / 2.f, (1.f - viewH) / 2.f, viewW, viewH));

    window_.setView(runnerView);
    window_.clear(sf::Color(135, 206, 235));
    drawBackground();

    switch (state_) {
    case GameState::Menu:       drawMenu(); break;
    case GameState::Playing:    drawGame(); drawHUD(); break;
    case GameState::Paused:     drawGame(); drawHUD(); drawPaused(); break;
    case GameState::LevelClear: drawGame(); drawHUD(); drawLevelClear(); break;
    case GameState::GameOver:   drawGame(); drawHUD(); drawGameOver(); break;
    case GameState::Win:        drawWin(); break;
    }

    window_.setView(window_.getDefaultView());
}

void LaneRunner::drawBackground() {
    if (hasBg_) {
        sf::Sprite bg(texBg_);
        sf::Vector2u sz = texBg_.getSize();
        bg.setScale(900.f / sz.x, 600.f / sz.y);
        window_.draw(bg);
        return;
    }

    sf::RectangleShape ground(sf::Vector2f(900.f, 600.f - GROUND_Y + 60.f));
    ground.setPosition(0.f, GROUND_Y - 10.f);
    ground.setFillColor(sf::Color(90, 60, 40));
    window_.draw(ground);

    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape track(sf::Vector2f(900.f, 6.f));
        track.setPosition(0.f, LANE_Y[i] + 40.f);
        track.setFillColor(sf::Color(60, 40, 25));
        window_.draw(track);
    }

    for (int i = -1; i < 15; ++i) {
        sf::RectangleShape stripe(sf::Vector2f(30.f, 8.f));
        stripe.setFillColor(sf::Color(200, 200, 60, 150));
        stripe.setPosition(i * 64.f - bgOffset_, GROUND_Y + 30.f);
        window_.draw(stripe);
    }
}

void LaneRunner::drawGame() {
    for (auto& e : entities_) {
        if (e.collected) continue;
        float ty = trackYFor(e.lane);

        if (e.type == EntityType::Coin) {
            float cy = ty - (e.tall ? 110.f : 60.f);
            if (hasCoin_) {
                sf::Sprite c(texCoin_);
                sf::Vector2u sz = texCoin_.getSize();
                float s = 30.f / std::max(sz.x, sz.y);
                c.setScale(s, s);
                c.setOrigin(static_cast<float>(sz.x) / 2.f, static_cast<float>(sz.y) / 2.f);
                c.setPosition(e.x, cy);
                window_.draw(c);
            }
            else {
                sf::CircleShape c(14.f);
                c.setOrigin(14.f, 14.f);
                c.setFillColor(sf::Color(255, 215, 0));
                c.setOutlineColor(sf::Color(200, 150, 0));
                c.setOutlineThickness(2.f);
                c.setPosition(e.x, cy);
                window_.draw(c);
            }
        }
        else if (e.type == EntityType::PowerUp) {
            // slot for watermelon-piece art: IMAGES/LaneRunner/watermelon.png
            float wy = ty - 60.f;
            if (hasWatermelon_) {
                sf::Sprite w(texWatermelon_);
                sf::Vector2u sz = texWatermelon_.getSize();
                float s = 34.f / std::max(sz.x, sz.y);
                w.setScale(s, s);
                w.setOrigin(static_cast<float>(sz.x) / 2.f, static_cast<float>(sz.y) / 2.f);
                w.setPosition(e.x, wy);
                window_.draw(w);
            }
            else {
                // placeholder: green rind circle with a pink wedge, so it
                // reads as "watermelon" even before the real art is added
                sf::CircleShape rind(17.f);
                rind.setOrigin(17.f, 17.f);
                rind.setFillColor(sf::Color(60, 170, 90));
                rind.setPosition(e.x, wy);
                window_.draw(rind);

                sf::CircleShape flesh(11.f);
                flesh.setOrigin(11.f, 11.f);
                flesh.setFillColor(sf::Color(230, 70, 90));
                flesh.setPosition(e.x, wy);
                window_.draw(flesh);
            }
        }
        else if (e.type == EntityType::Bee) {
            // slot for animated bee art: IMAGES/LaneRunner/bee_strip.png
            // (a horizontal strip of BEE_FRAME_COUNT frames, wing-flap loop)
            float by = beeYFor(e);
            if (hasBee_ && beeFrameW_ > 0 && beeFrameH_ > 0) {
                sf::Sprite b(texBee_);
                int idx = beeAnimFrame_ % BEE_FRAME_COUNT;
                b.setTextureRect(sf::IntRect(idx * beeFrameW_, 0, beeFrameW_, beeFrameH_));
                float s = BEE_SIZE / std::max(beeFrameW_, beeFrameH_);
                b.setScale(s, s);
                b.setOrigin(static_cast<float>(beeFrameW_) / 2.f, static_cast<float>(beeFrameH_) / 2.f);
                b.setPosition(e.x, by);
                window_.draw(b);
            }
            else {
                // placeholder: yellow/black striped body + two wing ovals
                // that flap using beeAnimFrame_ so it still reads as animated
                bool wingsUp = (beeAnimFrame_ % 2 == 0);
                float wingSpread = wingsUp ? 14.f : 9.f;

                sf::CircleShape wingL(8.f, 3);
                wingL.setScale(1.4f, 0.7f);
                wingL.setOrigin(8.f, 8.f);
                wingL.setFillColor(sf::Color(255, 255, 255, 180));
                wingL.setPosition(e.x - wingSpread, by - 10.f);
                window_.draw(wingL);

                sf::CircleShape wingR(8.f, 3);
                wingR.setScale(1.4f, 0.7f);
                wingR.setOrigin(8.f, 8.f);
                wingR.setFillColor(sf::Color(255, 255, 255, 180));
                wingR.setPosition(e.x + wingSpread, by - 10.f);
                window_.draw(wingR);

                sf::CircleShape body(11.f);
                body.setScale(1.1f, 0.85f);
                body.setOrigin(11.f, 11.f);
                body.setFillColor(sf::Color(255, 200, 40));
                body.setPosition(e.x, by);
                window_.draw(body);

                sf::RectangleShape stripe(sf::Vector2f(22.f, 4.f));
                stripe.setOrigin(11.f, 2.f);
                stripe.setFillColor(sf::Color(30, 30, 30));
                stripe.setPosition(e.x, by);
                window_.draw(stripe);
            }
        }
        else if (e.tall) {
            // stretched down to the ground so the sprite doesn't float -
            // the collision box (see entityRect) is unchanged, still only
            // the top BEAM_H, so sliding under it still works
            if (hasTall_) {
                sf::Sprite s(texTall_);
                sf::Vector2u sz = texTall_.getSize();
                s.setScale(48.f / sz.x, BEAM_VISUAL_H / sz.y);
                s.setPosition(e.x - 24.f, ty - BEAM_VISUAL_H);
                window_.draw(s);
            }
            else {
                sf::RectangleShape r(sf::Vector2f(48.f, BEAM_VISUAL_H));
                r.setFillColor(sf::Color(150, 40, 40));
                r.setPosition(e.x - 24.f, ty - BEAM_VISUAL_H);
                window_.draw(r);
            }
        }
        else {
            if (hasLow_) {
                sf::Sprite s(texLow_);
                sf::Vector2u sz = texLow_.getSize();
                s.setScale(56.f / sz.x, HURDLE_H / sz.y);
                s.setPosition(e.x - 28.f, ty - HURDLE_H);
                window_.draw(s);
            }
            else {
                sf::RectangleShape r(sf::Vector2f(56.f, HURDLE_H));
                r.setFillColor(sf::Color(80, 80, 200));
                r.setPosition(e.x - 28.f, ty - HURDLE_H);
                window_.draw(r);
            }
        }
    }

    float feetY = player_.y + 40.f - player_.jumpArc;

    // invincibility glow, drawn behind the player - pulses using fxClock_
    if (player_.invincible) {
        float pulse = 0.5f + 0.5f * std::sin(fxClock_ * 10.f);
        sf::CircleShape glow(32.f);
        glow.setOrigin(32.f, 32.f);
        glow.setPosition(PLAYER_X, feetY - 45.f);
        glow.setFillColor(sf::Color(255, 255, 60, static_cast<sf::Uint8>(90 + 90 * pulse)));
        window_.draw(glow);
    }

    // Running in-game, idle everywhere else - both strips animate.
    bool useRun = (state_ == GameState::Playing) && hasRun_;
    const sf::Texture* animTex = useRun ? &texRun_ : (hasIdle_ ? &texIdle_ : nullptr);
    int fw = useRun ? runFrameW_ : idleFrameW_;
    int fh = useRun ? runFrameH_ : idleFrameH_;
    int count = useRun ? runFrameCount_ : idleFrameCount_;
    float contentH = useRun ? runContentH_ : idleContentH_;
    float contentBottom = useRun ? runContentBottom_ : idleContentBottom_;

    if (animTex && fw > 0 && fh > 0 && count > 0 && contentH > 0.f) {
        int idx = animFrame_ % count;
        playerSprite_.setTexture(*animTex);
        playerSprite_.setTextureRect(sf::IntRect(idx * fw, 0, fw, fh));
        float h = player_.sliding ? PLAYER_SLIDE_H : PLAYER_STAND_H;
        float s = h / contentH;   // scale by the real visible height
        playerSprite_.setScale(s, s);
        // frame cell is bigger than the character, so anchor the content's
        // bottom edge on the track instead of the (mostly transparent) frame
        playerSprite_.setPosition(PLAYER_X - fw * s / 2.f, feetY - contentBottom * s);
        playerSprite_.setColor(player_.invincible ? sf::Color(255, 255, 200) : sf::Color::White);
        window_.draw(playerSprite_);
    }
    else {
        float h = player_.sliding ? PLAYER_SLIDE_H - 2.f : 70.f;
        player_.body.setSize(sf::Vector2f(46.f, h));
        player_.body.setOrigin(23.f, h);
        player_.body.setPosition(PLAYER_X, feetY);
        sf::Color bodyColor = player_.sliding ? sf::Color(240, 170, 60) : sf::Color(240, 90, 60);
        if (player_.invincible) bodyColor = sf::Color(255, 235, 120);
        player_.body.setFillColor(bodyColor);
        window_.draw(player_.body);

        player_.head.setPosition(PLAYER_X, feetY - h - 10.f);
        window_.draw(player_.head);
    }
}

void LaneRunner::drawHUD() {
    sf::Text lvl("Level " + std::to_string(level_) + " / " + std::to_string(TOTAL_LEVELS), font_, 22);
    lvl.setFillColor(sf::Color::White);
    lvl.setOutlineColor(sf::Color::Black);
    lvl.setOutlineThickness(2.f);
    lvl.setPosition(15.f, 12.f);
    window_.draw(lvl);

    sf::Text coinTxt("Coins: " + std::to_string(coinsThisLevel_) + " / " + std::to_string(COINS_PER_LEVEL), font_, 22);
    coinTxt.setFillColor(sf::Color::White);
    coinTxt.setOutlineColor(sf::Color::Black);
    coinTxt.setOutlineThickness(2.f);
    coinTxt.setPosition(15.f, 42.f);
    window_.draw(coinTxt);

    sf::Text total("Total Coins: " + std::to_string(totalCoins_), font_, 20);
    total.setFillColor(sf::Color::White);
    total.setOutlineColor(sf::Color::Black);
    total.setOutlineThickness(2.f);
    total.setPosition(900.f - 240.f, 12.f);
    window_.draw(total);

    sf::Text score("Score: " + std::to_string(score_), font_, 20);
    score.setFillColor(sf::Color::White);
    score.setOutlineColor(sf::Color::Black);
    score.setOutlineThickness(2.f);
    score.setPosition(15.f, 74.f);
    window_.draw(score);

    sf::RectangleShape barBg(sf::Vector2f(220.f, 16.f));
    barBg.setPosition(900.f - 240.f, 44.f);
    barBg.setFillColor(sf::Color(50, 50, 50, 180));
    window_.draw(barBg);
    float frac = std::min(1.f, static_cast<float>(coinsThisLevel_) / COINS_PER_LEVEL);
    sf::RectangleShape barFg(sf::Vector2f(220.f * frac, 16.f));
    barFg.setPosition(900.f - 240.f, 44.f);
    barFg.setFillColor(sf::Color(255, 215, 0));
    window_.draw(barFg);

    // invincibility timer readout
    if (player_.invincible) {
        sf::Text inv("INVINCIBLE: " + std::to_string(static_cast<int>(std::ceil(player_.invincibleTimer))) + "s",
            font_, 20);
        inv.setFillColor(sf::Color(255, 255, 80));
        inv.setOutlineColor(sf::Color::Black);
        inv.setOutlineThickness(2.f);
        inv.setPosition(15.f, 104.f);
        window_.draw(inv);
    }
}

void LaneRunner::centeredText(const std::string& str, int size, float y, sf::Color color) {
    sf::Text t(str, font_, size);
    t.setFillColor(color);
    t.setOutlineColor(sf::Color::Black);
    t.setOutlineThickness(3.f);
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin(b.width / 2.f, b.height / 2.f);
    t.setPosition(450.f, y);
    window_.draw(t);
}

void LaneRunner::drawOverlay(sf::Color c) {
    sf::RectangleShape ov(sf::Vector2f(900.f, 600.f));
    ov.setFillColor(c);
    window_.draw(ov);
}

void LaneRunner::drawMenu() {
    drawOverlay(sf::Color(0, 0, 0, 120));
    centeredText("LANE RUNNER", 56, 600.f * 0.30f, sf::Color(255, 215, 0));
    centeredText("Subway-style Endless Runner - 5 Levels", 20, 600.f * 0.40f);
    centeredText("UP/DOWN or W/S: Change Lane", 20, 600.f * 0.52f);
    centeredText("SPACE: Jump    SHIFT: Slide", 20, 600.f * 0.58f);
    centeredText("Watermelon = 5s invincibility. Bees always kill - dodge by lane!", 18, 600.f * 0.64f);
    centeredText("Press SPACE / ENTER to Start", 26, 600.f * 0.76f, sf::Color(120, 255, 120));
    centeredText("ESC to go back", 16, 600.f * 0.84f);
}

void LaneRunner::drawPaused() {
    drawOverlay(sf::Color(0, 0, 0, 150));
    centeredText("PAUSED", 48, 600.f * 0.42f, sf::Color(255, 255, 0));
    centeredText("Press P to resume", 22, 600.f * 0.52f);
}

void LaneRunner::drawLevelClear() {
    drawOverlay(sf::Color(0, 0, 0, 150));
    centeredText("LEVEL " + std::to_string(level_) + " COMPLETE!", 42, 600.f * 0.40f, sf::Color(120, 255, 120));
    centeredText("Coins collected: " + std::to_string(totalCoins_), 22, 600.f * 0.50f);
    centeredText("Press SPACE / ENTER to continue", 22, 600.f * 0.60f, sf::Color(255, 255, 255));
}

void LaneRunner::drawGameOver() {
    drawOverlay(sf::Color(80, 0, 0, 170));
    centeredText("GAME OVER", 52, 600.f * 0.28f, sf::Color(255, 80, 80));
    centeredText("Score: " + std::to_string(score_), 24, 600.f * 0.38f);
    centeredText("Reached Level " + std::to_string(level_) + "   Coins: " + std::to_string(totalCoins_), 22, 600.f * 0.46f);
    centeredText("Your name: " + nameEntry_ + (nameEntry_.size() < 12 ? "_" : ""), 24, 600.f * 0.56f, sf::Color(255, 255, 120));
    centeredText("Type to edit  -  Press SPACE / ENTER to retry", 20, 600.f * 0.64f, sf::Color(255, 255, 255));
}

void LaneRunner::drawWin() {
    window_.clear(sf::Color(20, 20, 60));
    centeredText("YOU WIN!", 60, 600.f * 0.30f, sf::Color(255, 215, 0));
    centeredText("All 5 levels complete!", 26, 600.f * 0.40f);
    centeredText("Score: " + std::to_string(score_) + "   Total Coins: " + std::to_string(totalCoins_), 24, 600.f * 0.48f);
    centeredText("Press SPACE / ENTER to return to Menu", 22, 600.f * 0.64f, sf::Color(120, 255, 120));
}