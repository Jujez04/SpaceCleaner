#pragma once
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "game/Collision.h"

// Forward declarations per ridurre le dipendenze circolari
class Entity;

/**
 * @brief Contiene informazioni di rendering per una sotto-mesh (SubMesh)
 *
 * Ogni entità può avere più SubMesh da renderizzare, ciascuna con un proprio
 * shader, colore e trasformazione locale.
 */
struct SubMeshRenderInfo {
    unsigned int meshId;                 ///< Identificativo della mesh (gestito dal MeshManager)
    unsigned int shaderId;               ///< Identificativo dello shader da usare per il rendering
    glm::vec4 color;                     ///< Colore della mesh (RGBA)
    bool visible = true;                 ///< Indica se la mesh è visibile
    glm::mat4 localTransform = glm::mat4(1.0f); ///< Trasformazione locale della SubMesh

    /**
     * @brief Costruttore base per una SubMesh.
     * @param mId ID della mesh
     * @param sId ID dello shader
     * @param col Colore iniziale
     */
    SubMeshRenderInfo(unsigned int mId, unsigned int sId, const glm::vec4& col)
        : meshId(mId), shaderId(sId), color(col) {
    }
};

/**
 * @brief Classe base per tutti i componenti di un'entità.
 *
 * Tutti i componenti derivano da questa classe e possiedono un riferimento
 * al proprietario (`Entity`) e un flag di abilitazione.
 */
class Component {
public:
    virtual ~Component() = default;

    bool enabled = true;     ///< Indica se il componente è attivo
    Entity* owner = nullptr; ///< Puntatore all'entità proprietaria
};

/**
 * @brief Componente che gestisce posizione, rotazione e scala di un'entità.
 *
 * Fornisce metodi per manipolare la trasformazione e calcolare la matrice modello.
 */
class TransformComponent : public Component {
private:
    glm::vec2 position; ///< Posizione 2D dell'entità
    glm::vec2 scale;    ///< Scala dell'entità
    float rotation;     ///< Rotazione in radianti

public:
    /**
     * @brief Costruttore del TransformComponent.
     * @param pos Posizione iniziale
     * @param rot Rotazione iniziale
     * @param scl Scala iniziale
     */
    TransformComponent(
        const glm::vec2& pos = glm::vec2(0.0f),
        float rot = 0.0f,
        const glm::vec2& scl = glm::vec2(1.0f)
    ) : position(pos), rotation(rot), scale(scl) {
    }

    ~TransformComponent() override = default;

    /**
     * @brief Restituisce la matrice modello (Model Matrix) calcolata da posizione, rotazione e scala.
     */
    const glm::mat4 getModelMatrix() const;

    // Getters
    const glm::vec2& getPosition() const { return position; }
    float getRotation() const { return rotation; }
    const glm::vec2& getScale() const { return scale; }

    // Setters
    void setPosition(const glm::vec2& pos);
    void setRotation(float rot);
    void setScale(const glm::vec2& scl);

    // Utility methods
    void translate(const glm::vec2& offset);
    void rotate(float angle);
    void scaleBy(const glm::vec2& factor);
};

/**
 * @brief Componente responsabile della gestione del rendering di un'entità.
 *
 * Contiene una lista di SubMeshRenderInfo che specificano le varie mesh e shader da renderizzare.
 */
class RenderComponent : public Component {
private:
    std::vector<SubMeshRenderInfo> subMeshes; ///< Lista delle sotto-mesh da disegnare

public:
    RenderComponent() = default;
    ~RenderComponent() override = default;

    /**
     * @brief Aggiunge una SubMesh da renderizzare.
     * @param meshInfo Riferimento alle informazioni della SubMesh
     */
    void addSubMesh(SubMeshRenderInfo& meshInfo);

    /**
     * @brief Restituisce l'elenco completo delle SubMesh.
     */
    const std::vector<SubMeshRenderInfo>& getSubMeshes() const;

    /**
     * @brief Rimuove tutte le SubMesh registrate.
     */
    void clearSubMesh();
};

/**
 * @brief Classe base per tutte le entità del gioco (giocatore, nemici, proiettili, ecc.)
 *
 * Ogni entità possiede un identificativo unico, un nome, e una serie di componenti.
 * La classe è astratta e deve essere derivata per implementare il comportamento (`update`).
 */
class Entity {
private:
    static int nextId;  ///< Contatore statico per assegnare ID univoci
    int id;             ///< Identificativo dell'entità
    std::string name;   ///< Nome leggibile dell'entità
    bool active;        ///< Stato di attivazione

public:
    // Componenti principali (composition-based)
    TransformComponent transform;  ///< Gestisce posizione, rotazione e scala
    RenderComponent renderData;    ///< Contiene le informazioni di rendering

    // Costruttori e distruttori
    explicit Entity(const std::string& entityName = "Entity");
    virtual ~Entity() = default;

    // Evita copia per mantenere unicità dell'entità
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    // Abilita lo spostamento (move semantics)
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;

    // Gestione rendering
    void addMeshLayer(SubMeshRenderInfo meshInfo);
    void clearMeshLayers();

    // Metodi virtuali base
    virtual void update(float deltaTime) = 0;   ///< Aggiorna lo stato logico dell'entità
    virtual void onCollision(Entity* other) {}  ///< Gestisce eventuali collisioni
    virtual AABB getAABB() const;               ///< Restituisce il bounding box dell'entità

    // Getter e Setter
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    bool isActive() const { return active; }
    TransformComponent getTransform() const { return transform; }

    void setActive(bool state) { active = state; }
    void setName(const std::string& newName) { name = newName; }
};
