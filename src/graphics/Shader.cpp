#include "graphics/Shader.h"

#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/Renderer.h"
#include "utilities/Utilities.h"

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
{
    rendererId = createShader(vertexShader, fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(rendererId);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(getUniformLocationCached(name), v0, v1, v2, v3);
}

void Shader::setUniformVec4(const std::string& name, glm::vec4 vec) {

}

void Shader::setUniformMat4(const std::string& name, const glm::mat4& matrix) {
    // Prende la posizione dell'uniform nello shader
    // e applica la matrice
    glUniformMatrix4fv(getUniformLocationCached(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

bool Shader::compileShader(unsigned int shader, const std::string& type) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> infoLog(length);
        glGetShaderInfoLog(shader, length, &length, infoLog.data());

        std::cerr << "ERROR: Shader compilation failed (" << type << ")\n"
            << infoLog.data() << "\n";
        return false;
    }
    return true;
}

unsigned int Shader::createShader(const std::string& vertexCode, const std::string& fragmentCode) {
    const char* vCode = vertexCode.c_str();
    const char* fCode = fragmentCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vCode, nullptr);
    glCompileShader(vertexShader);
    if (!compileShader(vertexShader, "VERTEX")) {
        glDeleteShader(vertexShader);
        return 0;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fCode, nullptr);
    glCompileShader(fragmentShader);
    if (!compileShader(fragmentShader, "FRAGMENT")) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    assignId();
    glAttachShader(rendererId, vertexShader);
    glAttachShader(rendererId, fragmentShader);
    glLinkProgram(rendererId);

    // Check linking errors
    int success;
    glGetProgramiv(rendererId, GL_LINK_STATUS, &success);
    if (!success) {
        int length;
        glGetProgramiv(rendererId, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> infoLog(length);
        glGetProgramInfoLog(rendererId, length, &length, &infoLog[0]);
        std::cerr << "ERROR: Shader linking failed\n" << &infoLog[0] << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return rendererId;
}

void Shader::bind() {
    glUseProgram(rendererId);
}

void Shader::unbind()
{
    glUseProgram(0);
}

int Shader::getUniformLocationCached(const std::string& name) const {
    auto it = uniformLocationCache.find(name);
    if (it != uniformLocationCache.end())
        return it->second;

    int location = glGetUniformLocation(rendererId, name.c_str());
    uniformLocationCache[name] = location;
    return location;
}