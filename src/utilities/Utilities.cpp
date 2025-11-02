#include "utilities/Utilities.h"

std::string readFile(const std::string& filePath) {

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Errore nell'aprire il file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool loadCatmullRomFile(const std::string& path,
    std::vector<glm::vec2>& points,
    std::vector<TBCParams>& params)
{
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        float x, y, z, t, b, c;
        if (!(iss >> x >> y >> z >> t >> b >> c)) continue;
        points.emplace_back(x, y);
        params.push_back({ t, b, c });
    }
    return true;
}