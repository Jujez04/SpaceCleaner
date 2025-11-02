#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "math/CatmullRom.h"

std::string readFile(const std::string& filePath);

bool loadCatmullRomFile(const std::string& path, std::vector<glm::vec2>& points, std::vector<TBCParams>& params);