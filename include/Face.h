#pragma once

#include <vector>
#include <array>

struct Face {
    std::array<int, 3> vertexIndices;
    std::array<int, 3> normalIndices;
};
