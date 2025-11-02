#pragma once
#include <string>

struct BackgroundConfig {
    std::string name;             
    std::string fragmentShaderPath;
    unsigned int shaderId = 0;
};