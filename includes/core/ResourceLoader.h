#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

#include "graphics/ShaderManager.h"
#include "graphics/MeshManager.h"
#include "math/HermiteMesh.h"
#include "core/PlayerConfig.h"
#include "core/BackGround.h"
#include "utilities/Utilities.h"

class ResourceLoader {
public:
    ResourceLoader() = default;
    ~ResourceLoader() = default;

    // Carica TUTTE le risorse (mesh, shader, configurazioni, ecc.)
    void loadAllResources(
        unsigned int& defaultShaderId,
        unsigned int& projectileMeshId,
        unsigned int& heartMeshId,
        unsigned int& backgroundMeshId,
        std::vector<PlayerConfig>& playerConfigs,
        std::vector<BackgroundConfig>& backgroundConfigs
    );

private:
    // Funzioni interne modulari
    unsigned int loadDefaultShader();
    void loadPlayerConfigs(std::vector<PlayerConfig>& playerConfigs, unsigned int defaultShaderId);
    void loadGameMeshes(unsigned int& projectileMeshId, unsigned int& heartMeshId, unsigned int& backgroundMeshId);
    void loadBackgrounds(std::vector<BackgroundConfig>& backgroundConfigs, unsigned int& backgroundMeshId);

    // Utility privata per leggere file GLSL
    std::string readShaderFile(const std::string& path);
};