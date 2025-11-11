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

/**
 * @class ResourceLoader
 * @brief Classe responsabile del caricamento di tutte le risorse di gioco
 * 
 * Centralizza il caricamento di:
 * - Shader (default, background)
 * - Mesh (player, proiettili, UI, background)
 * - Configurazioni (modelli navicelle, sfondi animati)
 * 
 * Pattern utilizzato: Facade + Builder
 * - Espone un'interfaccia semplice (loadAllResources)
 * - Delega a metodi privati specializzati
 */
class ResourceLoader {
public:
    // Costruttore e distruttore di default
    ResourceLoader() = default;
    ~ResourceLoader() = default;

    /**
     * @brief Carica TUTTE le risorse di gioco in una singola chiamata
     * 
     * Questa funzione è il punto di ingresso principale e orchestra
     * il caricamento di tutte le risorse necessarie al gioco.
     * 
     * @param[out] defaultShaderId ID dello shader di default per il rendering
     * @param[out] projectileMeshId ID della mesh dei proiettili
     * @param[out] heartMeshId ID della mesh dei cuori (UI salute)
     * @param[out] backgroundMeshId ID della mesh quad per lo sfondo
     * @param[out] playerConfigs Vettore delle configurazioni delle navicelle
     * @param[out] backgroundConfigs Vettore delle configurazioni degli sfondi
     * 
     * @note L'ordine di caricamento è importante:
     *       1. Default shader (necessario per player configs)
     *       2. Game meshes (geometrie base)
     *       3. Player configs (richiedono default shader)
     *       4. Background configs (richiedono background mesh)
     */
    void loadAllResources(
        unsigned int& defaultShaderId,
        unsigned int& projectileMeshId,
        unsigned int& heartMeshId,
        unsigned int& backgroundMeshId,
        std::vector<PlayerConfig>& playerConfigs,
        std::vector<BackgroundConfig>& backgroundConfigs
    );

private:
    // FUNZIONI DI CARICAMENTO SPECIALIZZATE

    /**
     * @brief Carica lo shader di default usato per il rendering standard
     * @return ID dello shader caricato
     * 
     * Legge vertex.glsl e fragment.glsl e li compila in un programma shader.
     */
    unsigned int loadDefaultShader();
    
    /**
     * @brief Carica tutte le configurazioni delle navicelle giocabili
     * @param[out] playerConfigs Vettore da riempire con le configurazioni
     * @param[in] defaultShaderId Shader da usare per il rendering delle navi
     * 
     * Carica 3 modelli di navicella (SW, X-Wing, BaseShip), ciascuno con:
     * - Base (struttura principale)
     * - Livery (decorazione)
     * - Cockpit (cabina)
     */
    void loadPlayerConfigs(std::vector<PlayerConfig>& playerConfigs, unsigned int defaultShaderId);
    
    /**
     * @brief Carica le mesh di gioco (non-player)
     * @param[out] projectileMeshId Mesh del proiettile (rombo)
     * @param[out] heartMeshId Mesh del cuore per UI salute
     * @param[out] backgroundMeshId Mesh quad per lo sfondo
     * 
     * Utilizza curve di Hermite per generare forme procedurali.
     */
    void loadGameMeshes(unsigned int& projectileMeshId, unsigned int& heartMeshId, unsigned int& backgroundMeshId);
    
    /**
     * @brief Carica le configurazioni degli sfondi animati
     * @param[out] backgroundConfigs Vettore delle configurazioni background
     * @param[in] backgroundMeshId Mesh quad su cui applicare gli shader
     * 
     * Pre-compila tutti gli shader di background disponibili.
     */
    void loadBackgrounds(std::vector<BackgroundConfig>& backgroundConfigs, unsigned int& backgroundMeshId);

    /**
     * @brief Utility per leggere il contenuto di un file shader
     * @param path Percorso del file GLSL
     * @return Contenuto del file come stringa
     * 
     * Wrapper attorno alla funzione readFile() delle utilities.
     */
    std::string readShaderFile(const std::string& path);
};
