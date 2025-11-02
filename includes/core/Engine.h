#pragma once
#include <memory>
#include <vector>
#include <string>
#include "core/BackGround.h" 
#include "core/PlayerConfig.h"
#include "core/ScoreManager.h"

class Window;
class Renderer;
class Camera;
class SpaceCleaner;
class Scene;
class ImGuiManager;

enum GameState {
    PLAYING,
    GAME_OVER
};

class Engine {
public:
    std::unique_ptr<ImGuiManager> imguiManager;

    Engine();
    ~Engine();
    void init();
    void gameLoop();
    Renderer* getRenderer() const { return renderer.get(); }
    void adjustScore(int delta) { scoreManager.adjustScore(delta); }
    int getScore() const { return scoreManager.getScore(); }
    std::vector<BackgroundConfig> backgroundConfigs;
    std::vector<PlayerConfig> playerConfigs;
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Scene> scene;
    std::shared_ptr<SpaceCleaner> player;
    GameState currentState = PLAYING;
    ScoreManager scoreManager;
    bool imguiVisible = false;
    unsigned int defaultShaderId = 0;
    unsigned int projectileMeshId = 0;
    double lastFrameTime = 0.0;
    float fireCooldown = 0.2f;
    float timeSinceLastShot = 0.0f;
    unsigned int asteroidMeshId = 0;
    unsigned int cometMeshId = 0;  
    unsigned int heartMeshId = 0;
    unsigned int backgroundMeshId = 0;
    unsigned int backgroundShaderId = 0;
    unsigned int gameOverMeshId = 0;
    unsigned int boundingBoxMeshId = 0;
    void applyPlayerConfig(unsigned int configIndex);
    void processInput();
    void update(float delta);
    void rendering();
    void resetGame();

};
