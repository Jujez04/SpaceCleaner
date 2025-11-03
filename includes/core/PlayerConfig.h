#pragma once
#include <string>
#include <glm/glm.hpp>

// --- Struttura per i dettagli di rendering di un singolo strato ---
struct SubMeshInfo {
    std::string name;             // Usato solo per riferimento/debug se necessario
    glm::vec4 color;              // Colore del layer
    glm::mat4 localTransform;     // Trasformazione (scala, offset)
};

// --- Struttura per la configurazione completa della nave ---
struct PlayerConfig {
    std::string name;             // Nome del modello (es. "Star Cruiser A")

    // ID delle mesh specifiche per questo modello, da assegnare al momento della creazione
    unsigned int baseMeshId;
    unsigned int liveryMeshId;
    unsigned int cockpitMeshId;

    // Dettagli di rendering (colore e trasformazione)
    SubMeshInfo base;
    SubMeshInfo livery;
    SubMeshInfo cockpit;

    /**
     * @brief Costruttore per PlayerConfig.
     *
     * @param modelName Nome della configurazione.
     * @param bId ID della mesh base.
     * @param lId ID della mesh livrea.
     * @param cId ID della mesh cockpit.
     * @param baseInfo Info per il rendering del base layer.
     * @param liveryInfo Info per il rendering del livery layer.
     * @param cockpitInfo Info per il rendering del cockpit layer.
     */
    PlayerConfig(
        const std::string& modelName,
        unsigned int bId,
        unsigned int lId,
        unsigned int cId,
        const SubMeshInfo& baseInfo,
        const SubMeshInfo& liveryInfo,
        const SubMeshInfo& cockpitInfo)
        : name(modelName),
        baseMeshId(bId),
        liveryMeshId(lId),
        cockpitMeshId(cId),
        base(baseInfo),
        livery(liveryInfo),
        cockpit(cockpitInfo) {
    }
};