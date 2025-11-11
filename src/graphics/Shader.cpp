/**
 * @file Shader.cpp
 * @brief Implementazione della classe Shader.
 */

#include "graphics/Shader.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "utilities/Utilities.h"

 /**
  * @brief Costruttore: compila e linka un programma shader dai codici sorgente.
  * @param vertexShader Codice sorgente dello shader vertex.
  * @param fragmentShader Codice sorgente dello shader fragment.
  */
Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader) {
    rendererId = createShader(vertexShader, fragmentShader);
}

/** @brief Distruttore: rimuove il programma shader da OpenGL. */
Shader::~Shader() {
    glDeleteProgram(rendererId);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(getUniformLocationCached(name), v0, v1, v2, v3);
}

void Shader::setUniformVec4(const std::string& name, const glm::vec4& vec) {
    glUniform4fv(getUniformLocationCached(name), 1, glm::value_ptr(vec));
}

void Shader::setUniformMat4(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(getUniformLocationCached(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform1f(const std::string& name, float value) {
    glUniform1f(getUniformLocationCached(name), value);
}

void Shader::setUniformVec2(const std::string& name, const glm::vec2& vec) {
    glUniform2fv(getUniformLocationCached(name), 1, glm::value_ptr(vec));
}

/**
 * @brief Compila uno shader e segnala eventuali errori di compilazione.
 * @param shader ID OpenGL dello shader da compilare.
 * @param type Tipo dello shader (VERTEX o FRAGMENT).
 * @return true se la compilazione ha avuto successo, false altrimenti.
 */
bool Shader::compileShader(unsigned int shader, const std::string& type) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> infoLog(length);
        glGetShaderInfoLog(shader, length, &length, infoLog.data());
        std::cerr << "ERROR: Shader compilation failed (" << type << ")\n" << infoLog.data() << "\n";
        return false;
    }
    return true;
}

/**
 * @brief Crea, compila e linka un programma shader completo.
 * @param vertexCode Codice sorgente dello shader vertex.
 * @param fragmentCode Codice sorgente dello shader fragment.
 * @return L’ID del programma shader creato.
 */
unsigned int Shader::createShader(const std::string& vertexCode, const std::string& fragmentCode) {
    const char* vCode = vertexCode.c_str();
    const char* fCode = fragmentCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vCode, nullptr);
    glCompileShader(vertexShader);
    if (!compileShader(vertexShader, "VERTEX")) return 0;

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fCode, nullptr);
    glCompileShader(fragmentShader);
    if (!compileShader(fragmentShader, "FRAGMENT")) return 0;

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> infoLog(length);
        glGetProgramInfoLog(program, length, &length, infoLog.data());
        std::cerr << "ERROR: Shader linking failed\n" << infoLog.data() << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

void Shader::bind() const { glUseProgram(rendererId); }
void Shader::unbind() { glUseProgram(0); }

/**
 * @brief Restituisce la location di una uniform, usando la cache per ottimizzare le chiamate OpenGL.
 * @param name Nome della variabile uniform.
 * @return Location della uniform.
 */
int Shader::getUniformLocationCached(const std::string& name) const {
    auto it = uniformLocationCache.find(name);
    if (it != uniformLocationCache.end()) return it->second;

    int location = glGetUniformLocation(rendererId, name.c_str());
    uniformLocationCache[name] = location;
    return location;
}
