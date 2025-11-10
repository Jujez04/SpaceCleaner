#pragma once
#include <memory>
#include <vector>
#include <string>
#include "core/BackGround.h" 
#include "core/PlayerConfig.h"
#include "core/ScoreManager.h"

/**
* Classe principale del motore di gioco.
* Gestisce l'inizializzazione, il ciclo di gioco (che usa un gameloop pattern)
* e fornisce l'accesso ai componenti principali come il renderer, la scena e la gestione del punteggio.
*/

// Forward declarations
class Window;
class Renderer;
class Camera;
class SpaceCleaner;
class Scene;
class ImGuiManager;

/**
* Stati del gioco -> Vengono usati per implementare una macchina a stati finiti (FSM)
*/
enum GameState {
    START,
    PLAYING,
    PAUSED,
    GAME_OVER
};

/**
* @brief Classe principale del motore di gioco.
*/
class Engine {
public:
    // Chiama l'API per ImGUI
    std::unique_ptr<ImGuiManager> imguiManager;
    // Variabili di configurazione
    std::vector<BackgroundConfig> backgroundConfigs;
    std::vector<PlayerConfig> playerConfigs;
    // Riferimento alla finestra di gioco
    std::unique_ptr<Window> window;

    // Costruttore e distruttore
    Engine();
    ~Engine();

	// Inizializza il motore di gioco
    void init();

	// Ciclo principale del gioco
    void gameLoop();

    // Funzione di reset
    void resetGame();

	//Getter e Setter
    Renderer* getRenderer() const { return renderer.get(); }
    void adjustScore(int delta) { scoreManager.adjustScore(delta); }
    int getScore() const { return scoreManager.getScore(); }
    GameState getCurrentState() const { return currentState;  }
    void setCurrentState(GameState state) { currentState = state; }


private:
	// Riferimenti ai componenti principali del motore
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Scene> scene;
    std::shared_ptr<SpaceCleaner> player;

	// Campo per lo stato corrente del gioco
    GameState currentState = START;

    // Gestore del punteggio
    ScoreManager scoreManager;

    // Flag per l'editor
    bool imguiVisible = false;
    
    // Variabili di temporizzazione
    double lastFrameTime = 0.0;
    float fireCooldown = 0.2f;
    float timeSinceLastShot = 0.0f;

	// Shader per il disegno di default e per il background
    unsigned int defaultShaderId = 0;
    unsigned int backgroundShaderId = 0;

	// Mesh IDs per gli oggetti di gioco
    unsigned int projectileMeshId = 0;
    unsigned int asteroidMeshId = 0;
    unsigned int cometMeshId = 0;  
    unsigned int heartMeshId = 0;
    unsigned int backgroundMeshId = 0;
    unsigned int gameOverMeshId = 0;
    unsigned int boundingBoxMeshId = 0;

	// Variabili di debouncing per input
    float f1Cooldown = 0.2f;
    float timeSinceLastF1 = 0.0f;
    float escCooldown = 0.2f;
    float timeSinceLastEsc = 0.0f;

	// Applica la configurazione del giocatore selezionata
    void applyPlayerConfig(unsigned int configIndex);

    // Funzioni per il game loop pattern
    void processInput();
    void update(float delta);
    void rendering();

};
