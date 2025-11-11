/**
 * @file Vertex.h
 * @brief Definizione delle strutture e classi per la gestione di vertici, VBO, VAO e IBO.
 *
 * Contiene le classi:
 * - Vertex: rappresenta un singolo vertice con posizione e colore.
 * - VertexBufferElement e VertexBufferLayout: descrivono il layout dei dati nei buffer di vertici.
 * - VertexBuffer: gestisce un Vertex Buffer Object (VBO) OpenGL.
 * - VertexArray: gestisce un Vertex Array Object (VAO) e il collegamento dei buffer.
 * - IndexBuffer: gestisce un Index Buffer Object (IBO) per il disegno con elementi indicizzati.
 */

#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace vrtx {

    /**
     * @struct Vertex
     * @brief Rappresenta un singolo vertice con posizione e colore.
     */
    struct Vertex {
        glm::vec2 position;   ///< Posizione 2D del vertice.
        glm::vec3 color;      ///< Colore RGB del vertice.

        /**
         * @brief Costruttore con valori di default.
         * @param pos Posizione del vertice (default (0,0)).
         * @param col Colore del vertice (default bianco).
         */
        Vertex(const glm::vec2& pos = glm::vec2(0.0f),
            const glm::vec3& col = glm::vec3(1.0f));
    };

    /**
     * @struct VertexBufferElement
     * @brief Descrive un singolo elemento del layout di un Vertex Buffer.
     */
    struct VertexBufferElement {
        unsigned int type;     ///< Tipo OpenGL dei dati (es. GL_FLOAT).
        unsigned int count;    ///< Numero di componenti (es. 3 per un vec3).
        bool normalized;       ///< Indica se i dati devono essere normalizzati.

        /**
         * @brief Costruttore.
         * @param type Tipo dei dati (es. GL_FLOAT).
         * @param count Numero di componenti.
         * @param normalized Indica se normalizzare i dati.
         */
        VertexBufferElement(unsigned int type, unsigned int count, bool normalized);

        /**
         * @brief Restituisce la dimensione in byte del tipo OpenGL.
         * @param type Tipo OpenGL (GL_FLOAT, GL_UNSIGNED_INT, GL_UNSIGNED_BYTE).
         * @return Dimensione in byte del tipo.
         */
        static unsigned int getSizeOfType(unsigned int type);
    };

    /**
     * @class VertexBufferLayout
     * @brief Gestisce il layout dei dati di un Vertex Buffer.
     */
    class VertexBufferLayout {
    private:
        std::vector<VertexBufferElement> elements; ///< Lista degli elementi del layout.
        unsigned int stride = 0;                   ///< Spazio in byte tra due vertici consecutivi.

    public:
        VertexBufferLayout() = default;
        ~VertexBufferLayout() = default;

        /**
         * @brief Aggiunge un nuovo elemento al layout.
         * @tparam T Tipo di dato (float, unsigned int, unsigned char).
         * @param count Numero di componenti per vertice.
         */
        template<typename T>
        void push(int count);

        /** @brief Specializzazione per float. */
        template<> void push<float>(int count);

        /** @brief Specializzazione per unsigned int. */
        template<> void push<unsigned int>(int count);

        /** @brief Specializzazione per unsigned char. */
        template<> void push<unsigned char>(int count);

        /** @return Stride del layout. */
        unsigned int getStride() const;

        /** @return Riferimento agli elementi del layout. */
        const std::vector<VertexBufferElement>& getElements() const;
    };

    /**
     * @class VertexBuffer
     * @brief Gestisce un Vertex Buffer Object (VBO) OpenGL.
     */
    class VertexBuffer {
    private:
        unsigned int rendererId; ///< Identificatore OpenGL del buffer.

    public:
        VertexBuffer() = default;

        /**
         * @brief Crea e inizializza un Vertex Buffer con dati statici.
         * @param data Puntatore ai dati dei vertici.
         * @param size Dimensione in byte dei dati.
         */
        VertexBuffer(const void* data, unsigned int size);

        /** @brief Distruttore: rilascia il buffer GPU. */
        ~VertexBuffer();

        /** @brief Collega il buffer come attivo. */
        void bind() const;

        /** @brief Scollega il buffer. */
        void unbind() const;
    };

    /**
     * @class VertexArray
     * @brief Gestisce un Vertex Array Object (VAO) OpenGL.
     */
    class VertexArray {
    private:
        unsigned int id; ///< Identificatore OpenGL del VAO.

    public:
        VertexArray();
        ~VertexArray();

        /** @brief Collega il VAO. */
        void bind() const;

        /** @brief Scollega il VAO. */
        void unbind() const;

        /**
         * @brief Associa un VertexBuffer al VAO con un determinato layout.
         * @param vb VertexBuffer da associare.
         * @param vbl Layout dei dati del buffer.
         */
        void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl);
    };

    /**
     * @class IndexBuffer
     * @brief Gestisce un Index Buffer Object (IBO) OpenGL per il rendering indicizzato.
     */
    class IndexBuffer {
    private:
        unsigned int rendererId; ///< ID OpenGL del buffer.
        unsigned int count;      ///< Numero di indici.

    public:
        /**
         * @brief Crea un Index Buffer e carica i dati nella GPU.
         * @param data Puntatore agli indici.
         * @param count Numero di indici.
         */
        IndexBuffer(const unsigned int* data, unsigned int count);

        /** @brief Distruttore: rilascia il buffer GPU. */
        ~IndexBuffer();

        /** @brief Collega il buffer come attivo. */
        void bind() const;

        /** @brief Scollega il buffer. */
        void unbind() const;
    };

} // namespace vrtx
