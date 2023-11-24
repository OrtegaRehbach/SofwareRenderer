#pragma once

#include "glm/glm.hpp"
#include <functional>
#include "Fragment.h"

using ShaderFunction = std::function<Fragment(const Fragment& fragment)>;

class Model {
public:
    Model(const std::vector<glm::vec3>& vertexBufferObject, const glm::mat4& modelMatrix, const ShaderFunction& shader)
        : vertexBufferObject(vertexBufferObject), modelMatrix(modelMatrix), shader(shader) {}
    std::vector<glm::vec3> vertexBufferObject;
    glm::mat4 modelMatrix;
    ShaderFunction shader;
};