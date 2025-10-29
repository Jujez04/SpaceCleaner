#include "graphics/Shader.h"

#include <vector>

#include "graphics/Renderer.h"
#include "utilities/Utilities.h"

Shader::Shader(const std::string& filepath) {
    ShaderProgramSource source = parseShaderFile(filepath);
    rendererId = createShader(source.VertexSource, source.FragmentSource);
}

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
{
    rendererId = createShader(vertexShader, fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(rendererId);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(getUniformLocation(name), 1, v0, v1, v2);
}

unsigned int Shader::getUniformLocation(const std::string& name)
{
    return glGetUniformLocation(rendererId, name.c_str());
}

bool Shader::compileShader(unsigned int shader, const std::string& type) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> infoLog(length);
        glGetShaderInfoLog(shader, length, &length, &infoLog[0]);
        std::cerr << "ERROR: Shader compilation failed (" << type << ")\n"
            << &infoLog[0] << "\n";
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

ShaderProgramSource Shader::parseShaderFile(const std::string& filePath) {
    std::ifstream stream(filePath);
    std::string line;
    std::stringstream ss[2];
    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            if (type != ShaderType::NONE)
                ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

void Shader::bind() {
    glUseProgram(rendererId);
}

void Shader::unbind()
{
    glUseProgram(0);
}
