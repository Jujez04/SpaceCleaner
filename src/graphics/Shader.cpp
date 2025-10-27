#include "graphics/Shader.h"
#include "utilities/Utilities.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    // Carica il codice dai file
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    const char* vCode = vertexCode.c_str();
    const char* fCode = fragmentCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vCode, nullptr);
    glCompileShader(vertexShader);
    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fCode, nullptr);
    glCompileShader(fragmentShader);
 
	assignId();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::setUniformMat4(const std::string& name, float v0, float v1, float v2, float v3) {
    int location = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &v0);
}

void Shader::use() {
    glUseProgram(id);
}