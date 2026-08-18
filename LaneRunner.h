#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include <string>


class LaneRunner {
public:
    explicit LaneRunner(sf::RenderWindow& window);

    void reset();
    void handleEvent(const sf::Event& event);
    void update(float dt);
    void render();

    bool exitRequested() const { return exitRequested_; }
    void clearExitRequested() { exitRequested_ = false; }

    void setCharacter(int index);               // use the hub's chosen character
    void setPlayerName(const std::string& name); // name saved to the leaderboard
    void setDifficulty(int level);              // 0 easy, 1 normal, 2 hard

private:
    enum class GameState { Menu, Playing, LevelClear, GameOver, Win, Paused };
    enum class EntityType { Obstacle, Coin, PowerUp, Bee };

    struct Player {
        int lane = 1;
        float y = 330.f;             // LANE_Y[1]
        float targetY = 330.f;
        bool jumping = false;
        bool sliding = false;
        float slideTimer = 0.f;
        float jumpArc = 0.f;

        // invincibility (from watermelon power-up) - protects against
        // Obstacle hits only, NEVER against the Bee.
        bool invincible = false;
        float invincibleTimer = 0.f;

        sf::RectangleShape body;
        sf::CircleShape head;

        void init();
        void moveLane(int dir);
        bool requestJump();
        void slide();
        void update(float dt);
        sf::FloatRect getHitbox() const;
    };

    struct Entity {
        EntityType type;
        int lane;
        float x;
        bool tall = false;    // coin: high arc; obstacle: slide under
        bool collected = false;
        float phase = 0.f;    // bee: personal phase offset for its bob animation
        sf::Shape* shapePtr = nullptr;
    };

    sf::RenderWindow& window_;
    sf::Font font_;
    GameState state_ = GameState::Menu;

    Player player_;
    std::deque<Entity> entities_;

    // optional art (IMAGES/LaneRunner/ or the shared character strips)
    sf::Texture texIdle_;   // selected character's idle strip
    sf::Texture texRun_;    // selected character's run strip
    sf::Texture texCoin_;
    sf::Texture texTall_;
    sf::Texture texLow_;
    sf::Texture texBg_;
    sf::Texture texWatermelon_;  // power-up art
    sf::Texture texBee_;         // animated bee strip
    sf::Sprite playerSprite_;
    bool hasIdle_ = false;
    bool hasRun_ = false;
    bool hasCoin_ = false;
    bool hasTall_ = false;
    bool hasLow_ = false;
    bool hasBg_ = false;
    bool hasWatermelon_ = false;
    bool hasBee_ = false;

    // sprite-strip frame layout (measured from the loaded strip, like the shooter)
    int idleFrameW_ = 0, idleFrameH_ = 0, idleFrameCount_ = 6;
    int runFrameW_ = 0, runFrameH_ = 0, runFrameCount_ = 6;
    float idleContentH_ = 1.f, idleContentBottom_ = 0.f;
    float runContentH_ = 1.f, runContentBottom_ = 0.f;

    // bee strip frame layout
    int beeFrameW_ = 612/5, beeFrameH_ =408 ;
    static constexpr int BEE_FRAME_COUNT = 5;

    // run/idle animation playback
    int animFrame_ = 0;
    float animTimer_ = 0.f;
    float animFrameDuration_ = 0.09f;

    // bee animation playback (separate clock, bee flaps faster than the runner)
    int beeAnimFrame_ = 0;
    float beeAnimTimer_ = 0.f;
    float beeAnimFrameDuration_ = 0.06f;

    // rising elapsed-time clock, used to pulse the invincibility glow and
    // to drive the bee's vertical bob - purely cosmetic timing
    float fxClock_ = 0.f;

    int level_ = 1;
    int coinsThisLevel_ = 0;
    int totalCoins_ = 0;
    int score_ = 0;
    bool savedThisRun_ = false;
    float distance_ = 0.f;
    float scrollSpeed_ = 340.f;
    float spawnTimer_ = 0.f;
    float spawnInterval_ = 1.1f;
    float bgOffset_ = 0.f;
    float realJumpHeight_ = 0.f;
    float realJumpVel_ = 0.f;

    int difficulty_ = 1;
    float speedFactor_ = 1.f;
    std::string playerName_;
    std::string nameEntry_;

    bool exitRequested_ = false;

    float randRange(float lo, float hi);
    int randInt(int lo, int hi);
    void saveScore();

    void loadAssets();
    void resetGame();
    void setupLevel();
    void spawnEntity();
    float trackYFor(int lane) const;
    float beeYFor(const Entity& e) const;
    sf::FloatRect entityRect(const Entity& e) const;
    void updatePlaying(float dt);
    void measureStrip(const sf::Texture& tex, int& contentTop, int& contentBottom);
    bool setStripTexture(sf::Texture& tex, const std::string& path,
        int& frameW, int& frameH, float& contentH,
        float& contentBottom, int frameCount);

    void drawBackground();
    void drawGame();
    void drawHUD();
    void centeredText(const std::string& str, int size, float y, sf::Color color = sf::Color::White);
    void drawOverlay(sf::Color c);
    void drawMenu();
    void drawPaused();
    void drawLevelClear();
    void drawGameOver();
    void drawWin();
};