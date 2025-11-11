#include "core/ResourceLoader.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

/**
 * Punto di ingresso per il caricamento delle risorse.
 * Segue un ordine preciso per rispettare le dipendenze.
 */
void ResourceLoader::loadAllResources(
    unsigned int& defaultShaderId,
    unsigned int& projectileMeshId,
    unsigned int& heartMeshId,
    unsigned int& backgroundMeshId,
    std::vector<PlayerConfig>& playerConfigs,
    std::vector<BackgroundConfig>& backgroundConfigs
) {
    // 1. Carica lo shader di default (NECESSARIO per step 3)
    defaultShaderId = loadDefaultShader();

    // 2. Carica mesh di gioco (indipendenti)
    loadGameMeshes(projectileMeshId, heartMeshId, backgroundMeshId);

    // 3. Carica configurazioni player (DIPENDE da defaultShaderId)
    loadPlayerConfigs(playerConfigs, defaultShaderId);

    // 4. Carica configurazioni background (DIPENDE da backgroundMeshId)
    loadBackgrounds(backgroundConfigs, backgroundMeshId);
}

/**
 * Carica e compila lo shader di default utilizzato per la maggior parte
 * degli oggetti del gioco (player, nemici, proiettili, UI).
 */
unsigned int ResourceLoader::loadDefaultShader() {
    // Legge i sorgenti shader da file
    std::string vertexCode = readShaderFile("resources/vertex.glsl");
    std::string fragmentCode = readShaderFile("resources/fragment.glsl");

    // Compila e registra lo shader nel ShaderManager
    return ShaderManager::load("DefaultShader", vertexCode, fragmentCode);
}

/**
 * Carica tutte le navicelle giocabili del gioco.
 *
 * ARCHITETTURA A LAYER:
 * Ogni navicella è composta da 3 layer sovrapposti:
 * 1. Base: Struttura principale (grigia/neutra)
 * 2. Livery: Decorazione/colore distintivo
 * 3. Cockpit: Cabina di pilotaggio (vetro trasparente)
 *
 * MODELLI DISPONIBILI:
 * - SW: Navicella ispirata a Star Wars (gialla)
 * - X-Wing: Fighter classico (rosso/blu)
 * - BaseShip: Nave standard (arancione)
 */
void ResourceLoader::loadPlayerConfigs(std::vector<PlayerConfig>& playerConfigs, unsigned int defaultShaderId) {
    // --- MODELLO SW ---
    // Carica le 3 componenti della navicella SW dai file di controllo points
    unsigned int swBaseId = HermiteMesh::catmullRomToMesh("SW_PlayerBase", "resources/BaseSWship.txt", 40);
    unsigned int swLiveryId = HermiteMesh::catmullRomToMesh("SW_PlayerLivery", "resources/LiverySWship.txt", 40);
    unsigned int swCockpitId = HermiteMesh::catmullRomToMesh("SW_PlayerCockpit", "resources/CockpitSWship.txt", 40);

    // --- MODELLO BASESHIP ---
    unsigned int baseShipBaseId = HermiteMesh::catmullRomToMesh("BaseShip_PlayerBase", "resources/BaseFirstShip.txt", 40);
    unsigned int baseShipLiveryId = HermiteMesh::catmullRomToMesh("BaseShip_PlayerLivery", "resources/LiveryBaseShip.txt", 40);
    unsigned int baseShipCockpitId = HermiteMesh::catmullRomToMesh("BaseShip_PlayerCockpit", "resources/CockpitFirstShip.txt", 40);

    // --- MODELLO X-WING ---
    // X-Wing ha più dettagli (100 e 90 segmenti per base e livery)
    unsigned int xwingShipBaseId = HermiteMesh::catmullRomToMesh("XWing_PlayerBase", "resources/BaseXwingShip.txt", 100);
    unsigned int xwingShipLiveryId = HermiteMesh::catmullRomToMesh("XWing_PlayerLivery", "resources/LiveryXwingShip.txt", 90);
    unsigned int xwingShipCockpitId = HermiteMesh::catmullRomToMesh("XWing_PlayerCockpit", "resources/CockpitXwingShip.txt", 40);

    // CONFIGURAZIONI

    // --- CONFIGURAZIONE SW ---
    PlayerConfig model1 = {
        "SW",                           // Nome del modello
        swBaseId, swLiveryId, swCockpitId,  // ID delle mesh

        // Layer Base: grigio neutro, scala 0.3
        { "PlayerBase",
          glm::vec4(0.5f),              // Colore grigio medio
          glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)) },

          // Layer Livery: giallo brillante, leggermente più grande e traslato
          { "PlayerLivery",
            glm::vec4(0.9f, 0.9f, 0.0f, 1.0f),  // Colore giallo
            glm::translate(
                glm::scale(glm::mat4(1.0f), glm::vec3(0.31f)),
                glm::vec3(-0.01f, 0.02f, 0.1f)    // Offset Z per sovrapposizione
            ) },

        // Layer Cockpit: quasi nero, trasparente al 70%
        { "PlayerCockpit",
          glm::vec4(0.1f, 0.1f, 0.1f, 0.7f),  // Grigio scuro trasparente
          glm::translate(
              glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)),
              glm::vec3(-0.04f, 0.1f, 0.2f)     // Z più alto per visibilità
          ) }
    };

    // --- CONFIGURAZIONE X-WING ---
    PlayerConfig model2 = {
        "X-Wing",
        xwingShipBaseId, xwingShipLiveryId, xwingShipCockpitId,

        // Base: grigio scuro, scala 0.4 (più grande)
        { "PlayerBase",
          glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
          glm::scale(glm::mat4(1.0f), glm::vec3(0.4f)) },

          // Livery: rosso scuro (tema X-Wing Rebel Alliance)
          { "PlayerLivery",
            glm::vec4(0.5f, 0.0f, 0.0f, 1.0f),
            glm::translate(
                glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)),
                glm::vec3(0.03f, -0.03f, 0.1f)
            ) },

        // Cockpit: blu traslucido (vetro colorato)
        { "PlayerCockpit",
          glm::vec4(0.2f, 0.2f, 0.8f, 0.7f),
          glm::translate(
              glm::scale(glm::mat4(1.0f), glm::vec3(0.6f)),
              glm::vec3(0.02f, -0.2f, 0.2f)
          ) }
    };

    // --- CONFIGURAZIONE BASESHIP ---
    PlayerConfig model3 = {
        "BaseShip",
        baseShipBaseId, baseShipLiveryId, baseShipCockpitId,

        // Base: rosso scuro
        { "PlayerBase",
          glm::vec4(0.7f, 0.7f, 0.7f, 1.0f),
          glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)) },

          // Livery: grigio medio
          { "PlayerLivery",
            glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
            glm::translate(
                glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)),
                glm::vec3(-0.05f, -0.1f, 0.05f)
            ) },

        // Cockpit: arancione trasparente
        { "PlayerCockpit",
          glm::vec4(1.0f, 0.8f, 0.0f, 0.7f),
          glm::translate(
              glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)),
              glm::vec3(-0.06f, -0.05f, 0.1f)
          ) }
    };

    // Popola il vettore di configurazioni (ordine menu)
    playerConfigs = { model1, model2, model3 };
}


/**
 * Genera proceduralmente le mesh utilizzate durante il gameplay e l'UI.
 *
 * Utilizza curve di Hermite per creare forme partendo dai punti di controllo.
 */
void ResourceLoader::loadGameMeshes(unsigned int& projectileMeshId, unsigned int& heartMeshId, unsigned int& backgroundMeshId) {
    // MESH PROIETTILE
    // Forma a rombo (diamante) per proiettili veloci e visibili
    std::vector<glm::vec2> projectilePoints = {
        {0.0f, 0.05f},      // Punta superiore
        {0.015f, 0.0f},     // Punta destra
        {0.0f, -0.05f},     // Punta inferiore
        {-0.015f, 0.0f}     // Punta sinistra
    };
    projectileMeshId = HermiteMesh::baseHermiteToMesh("ProjectileShape", projectilePoints, 10);

    // MESH CUORE (UI SALUTE)
    // Forma classica a cuore per indicatore salute
    // 9 punti di controllo per creare le curve caratteristiche
    std::vector<glm::vec2> heartPoints = {
        {0.003f, 0.16f},      // Centro superiore
        {0.124f, 0.301f},     // Lobo destro superiore
        {0.177f, 0.178f},     // Lobo destro laterale
        {0.186f, 0.012f},     // Curva destra
        {0.0013f, -0.42f},    // Punta inferiore
        {-0.186f, 0.012f},    // Curva sinistra
        {-0.177f, 0.178f},    // Lobo sinistro laterale
        {-0.124f, 0.301f},    // Lobo sinistro superiore
        {0.003f, 0.16f}       // Chiusura (ritorna al primo punto)
    };
    // 30 segmenti per curve smooth
    heartMeshId = HermiteMesh::baseHermiteToMesh("HeartShape", heartPoints, 30);

    // MESH BACKGROUND (QUAD FULLSCREEN)
    // Semplice quad che copre l'intera viewport per shader di background
    std::vector<glm::vec2> quadPoints = {
        {-1.0f, 1.0f},      // Top-left
        {1.0f, 1.0f},       // Top-right
        {1.0f, -1.0f},      // Bottom-right
        {-1.0f, -1.0f}      // Bottom-left
    };
    // 4 vertici = 2 triangoli = 1 quad
    backgroundMeshId = HermiteMesh::baseHermiteToMesh("BackgroundQuad", quadPoints, 4);
}

/**
 * Pre-compila tutti gli shader di background disponibili.
 *
 * Ogni background utilizza:
 * - Lo stesso vertex shader (background_vertex.glsl)
 * - Un fragment shader unico che crea l'effetto visivo
 * - La stessa mesh quad fullscreen
 */
void ResourceLoader::loadBackgrounds(std::vector<BackgroundConfig>& backgroundConfigs, unsigned int& backgroundMeshId) {
    // Legge il vertex shader comune a tutti i background
    std::string bgVertexCode = readShaderFile("resources/background_vertex.glsl");

	// CONFIGURAZIONI BACKGROUND DISPONIBILI
    backgroundConfigs = {
        // Background 1: Campo stellato semplice
        {"Default", "resources/defaultbg_fragment_shader.glsl", 0},

        // Background 2: Saturno
        {"Saturn", "resources/fragmentSaturn.glsl", 0},

        // Background 3: Sole
        {"Sun theme", "resources/fragmentSun.glsl", 0}
    };

    // PRE-COMPILAZIONE SHADER E CARICAMENTO NELLO SHADER MANAGER
    // Pre-carica e compila tutti gli shader per evitare lag durante il cambio
    for (auto& config : backgroundConfigs) {
        // Legge il fragment shader specifico
        std::string frag = readShaderFile(config.fragmentShaderPath);

        // Compila e registra lo shader, salvando l'ID nella config
        config.shaderId = ShaderManager::load(config.name, bgVertexCode, frag);
    }
}

/**
 * Wrapper per la lettura di file shader.
 * Delega alla funzione readFile() delle utilities del progetto.
 */
std::string ResourceLoader::readShaderFile(const std::string& path) {
    return readFile(path);
}