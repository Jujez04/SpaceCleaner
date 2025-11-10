#include "core/ResourceLoader.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

// Funzione principale che aggrega tutte le chiamate di caricamento.
void ResourceLoader::loadAllResources(
    unsigned int& defaultShaderId,
    unsigned int& projectileMeshId,
    unsigned int& heartMeshId,
    unsigned int& backgroundMeshId,
    std::vector<PlayerConfig>& playerConfigs,
    std::vector<BackgroundConfig>& backgroundConfigs
) {
    defaultShaderId = loadDefaultShader();
    loadGameMeshes(projectileMeshId, heartMeshId, backgroundMeshId);
    // Necessita di defaultShaderId caricato
    loadPlayerConfigs(playerConfigs, defaultShaderId);
    loadBackgrounds(backgroundConfigs, backgroundMeshId);
}

// -----------------------------------------------------
unsigned int ResourceLoader::loadDefaultShader() {
    // Utilizza la funzione privata per leggere i file shader
    std::string vertexCode = readShaderFile("resources/vertex.glsl");
    std::string fragmentCode = readShaderFile("resources/fragment.glsl");
    return ShaderManager::load("DefaultShader", vertexCode, fragmentCode);
}

// -----------------------------------------------------
void ResourceLoader::loadPlayerConfigs(std::vector<PlayerConfig>& playerConfigs, unsigned int defaultShaderId) {
    // Caricamento delle Mesh delle navi (SW, BaseShip, X-Wing)
    unsigned int swBaseId = HermiteMesh::catmullRomToMesh("SW_PlayerBase", "resources/BaseSWship.txt", 40);
    unsigned int swLiveryId = HermiteMesh::catmullRomToMesh("SW_PlayerLivery", "resources/LiverySWship.txt", 40);
    unsigned int swCockpitId = HermiteMesh::catmullRomToMesh("SW_PlayerCockpit", "resources/CockpitSWship.txt", 40);

    unsigned int baseShipBaseId = HermiteMesh::catmullRomToMesh("BaseShip_PlayerBase", "resources/BaseFirstShip.txt", 40);
    unsigned int baseShipLiveryId = HermiteMesh::catmullRomToMesh("BaseShip_PlayerLivery", "resources/LiveryBaseShip.txt", 40);
    unsigned int baseShipCockpitId = HermiteMesh::catmullRomToMesh("BaseShip_PlayerCockpit", "resources/CockpitFirstShip.txt", 40);

    unsigned int xwingShipBaseId = HermiteMesh::catmullRomToMesh("XWing_PlayerBase", "resources/BaseXwingShip.txt", 100);
    unsigned int xwingShipLiveryId = HermiteMesh::catmullRomToMesh("XWing_PlayerLivery", "resources/LiveryXwingShip.txt", 90);
    unsigned int xwingShipCockpitId = HermiteMesh::catmullRomToMesh("XWing_PlayerCockpit", "resources/CockpitXwingShip.txt", 40);

    // Definizione e configurazione dei Modelli
    PlayerConfig model1 = {
        "SW", swBaseId, swLiveryId, swCockpitId,
        { "PlayerBase", glm::vec4(0.5f), glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)) },
        { "PlayerLivery", glm::vec4(0.9f, 0.9f, 0.0f, 1.0f),
          glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.31f)), glm::vec3(-0.01f, 0.02f, 0.1f)) },
        { "PlayerCockpit", glm::vec4(0.1f, 0.1f, 0.1f, 0.7f),
          glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)), glm::vec3(-0.04f, 0.1f, 0.2f)) }
    };

    PlayerConfig model2 = {
        "X-Wing", xwingShipBaseId, xwingShipLiveryId, xwingShipCockpitId,
        { "PlayerBase", glm::vec4(0.3f), glm::scale(glm::mat4(1.0f), glm::vec3(0.4f)) },
        { "PlayerLivery", glm::vec4(0.5f, 0.0f, 0.0f, 1.0f),
          glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)), glm::vec3(0.03f, -0.03f, 0.1f)) },
        { "PlayerCockpit", glm::vec4(0.2f, 0.2f, 0.8f, 0.7f),
          glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.6f)), glm::vec3(0.02f, -0.2f, 0.2f)) }
    };

    PlayerConfig model3 = {
        "BaseShip", baseShipBaseId, baseShipLiveryId, baseShipCockpitId,
        { "PlayerBase", glm::vec4(0.0f), glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)) },
        { "PlayerLivery", glm::vec4(0.5f), glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)),
          glm::vec3(-0.05f, -0.1f, 0.05f)) },
        { "PlayerCockpit", glm::vec4(1.0f, 0.6f, 0.0f, 0.7f),
          glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)),
          glm::vec3(-0.06f, -0.05f, 0.1f)) }
    };

    playerConfigs = { model1, model2, model3 };
}

// -----------------------------------------------------
void ResourceLoader::loadGameMeshes(unsigned int& projectileMeshId, unsigned int& heartMeshId, unsigned int& backgroundMeshId) {
    // Mesh Proiettile
    std::vector<glm::vec2> projectilePoints = {
        {0.0f, 0.05f}, {0.015f, 0.0f}, {0.0f, -0.05f}, {-0.015f, 0.0f}
    };
    projectileMeshId = HermiteMesh::baseHermiteToMesh("ProjectileShape", projectilePoints, 10);

    // Mesh Cuore (UI)
    std::vector<glm::vec2> heartPoints = {
        {0.003f, 0.16f}, {0.124f, 0.301f}, {0.177f, 0.178f}, {0.186f, 0.012f},
        {0.0013f, -0.42f}, {-0.186f, 0.012f}, {-0.177f, 0.178f},
        {-0.124f, 0.301f}, {0.003f, 0.16f}
    };
    heartMeshId = HermiteMesh::baseHermiteToMesh("HeartShape", heartPoints, 30);

    // Mesh Quad (Sfondo)
    std::vector<glm::vec2> quadPoints = {
        {-1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, -1.0f}, {-1.0f, -1.0f}
    };
    backgroundMeshId = HermiteMesh::baseHermiteToMesh("BackgroundQuad", quadPoints, 4);
}

// -----------------------------------------------------
void ResourceLoader::loadBackgrounds(std::vector<BackgroundConfig>& backgroundConfigs, unsigned int& backgroundMeshId) {
    std::string bgVertexCode = readShaderFile("resources/background_vertex.glsl");

    // Definisci le configurazioni di background
    backgroundConfigs = {
        {"Default Blue", "resources/defaultbg_fragment_shader.glsl", 0},
        {"Stellar", "resources/fragmentSaturn.glsl", 0},
        {"Star Field", "resources/fragmentEarthOrbitalFlight.glsl", 0}
    };

    // Pre-carica tutti gli shader e assegna gli ID
    for (auto& config : backgroundConfigs) {
        std::string frag = readShaderFile(config.fragmentShaderPath);
        config.shaderId = ShaderManager::load(config.name, bgVertexCode, frag);
    }
}

// -----------------------------------------------------
std::string ResourceLoader::readShaderFile(const std::string& path) {
    // Delega la lettura del file all'utility (come da codice originale)
    return readFile(path);
}