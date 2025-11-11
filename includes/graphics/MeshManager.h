#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Mesh;

/**
 * @brief Gestore centralizzato per le mesh del motore grafico.
 *
 * Fornisce un sistema di registrazione e recupero delle mesh tramite nome o ID,
 * garantendo la condivisione efficiente delle risorse tra più entità.
 */
class MeshManager {
public:
    /**
     * @brief Registra una mesh identificata da un nome.
     *
     * Se una mesh con lo stesso nome esiste già, restituisce quella esistente.
     *
     * @param name Nome della mesh (chiave univoca)
     * @param vertices Vettore dei vertici
     * @param indices Vettore degli indici (opzionale)
     * @return std::shared_ptr<Mesh> Puntatore condiviso alla mesh
     */
    static std::shared_ptr<Mesh> registerMesh(
        const std::string& name,
        const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices = {}
    );

    /**
     * @brief Registra una mesh senza associarle un nome esplicito (solo tramite ID).
     *
     * @param vertices Vertici della mesh
     * @param indices Indici della mesh (opzionale)
     * @return std::shared_ptr<Mesh> Puntatore condiviso alla mesh registrata
     */
    static std::shared_ptr<Mesh> registerMeshById(
        const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices = {}
    );

    /// Restituisce la mesh registrata con un dato nome (nullptr se non trovata)
    static std::shared_ptr<Mesh> get(const std::string& name);

    /// Restituisce la mesh registrata con un dato ID (nullptr se non trovata)
    static std::shared_ptr<Mesh> getById(unsigned int id);

private:
    static std::unordered_map<std::string, std::shared_ptr<Mesh>> meshMap; ///< Mappa nome ? Mesh
    static std::unordered_map<unsigned int, std::shared_ptr<Mesh>> idMeshMap; ///< Mappa ID ? Mesh
    static unsigned int nextId; ///< ID progressivo assegnato alle nuove mesh
};
