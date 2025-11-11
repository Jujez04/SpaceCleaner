/**
 * @file Shader.h
 * @brief Dichiarazione della classe Shader, che incapsula la gestione di uno shader OpenGL.
 */

#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <unordered_map>

 /**
  * @class Shader
  * @brief Gestisce la creazione, compilazione e utilizzo di programmi shader OpenGL.
  *
  * Fornisce metodi per il binding e l’impostazione efficiente di uniform.
  */
class Shader {
private:
    unsigned int rendererId;  ///< Identificatore del programma shader OpenGL.
    mutable std::unordered_map<std::string, int> uniformLocationCache; ///< Cache per le uniform.

public:
    Shader() = default;
    Shader(const std::string& vertexShader, const std::string& fragmentShader);
    ~Shader();

    /** @brief Attiva lo shader per il rendering. */
    void bind() const;

    /** @brief Disattiva lo shader. */
    void unbind();

    // --- Uniform setters ---
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniformVec4(const std::string& name, const glm::vec4& vec);
    void setUniformMat4(const std::string& name, const glm::mat4& matrix);
    void setUniform1f(const std::string& name, float value);
    void setUniformVec2(const std::string& name, const glm::vec2& vec);

    /** @return ID OpenGL dello shader. */
    unsigned int getId() const { return rendererId; }

    /** @return true se lo shader è valido, false altrimenti. */
    bool isValid() const { return rendererId != 0; }

private:
    bool compileShader(unsigned int shader, const std::string& type);
    unsigned int createShader(const std::string& vertexCode, const std::string& fragmentCode);
    int getUniformLocationCached(const std::string& name) const;
};
