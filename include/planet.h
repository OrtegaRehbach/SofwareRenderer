#pragma once

#include <vector>
#include "glm/glm.hpp"

#include "model.h"
#include "RenderingUtils.h"

class Planet : public Model {
public:
    Planet(const std::vector<glm::vec3>& VBO, const glm::mat4& modelMatrix, const ShaderFunction& shader) 
        : Model(VBO, modelMatrix, shader) {};
    Planet(const std::vector<glm::vec3>& VBO, const glm::vec3& scale, const glm::vec3& position, const ShaderFunction& shader) 
        : Model(VBO, createModelMatrix(scale, position), shader), scale(scale), position(position) {};
    Planet(const std::vector<glm::vec3>& VBO, const glm::vec3& scale, const glm::vec3& position, const ShaderFunction& shader, float rotationSpeed, float orbitSpeed, float orbitRadius, const glm::vec3& orbitTarget = glm::vec3(0)) 
        : Model(VBO, createModelMatrix(scale, position), shader), scale(scale), position(position), rotationSpeed(rotationSpeed), 
        orbitSpeed(orbitSpeed), orbitRadius(orbitRadius),orbitTarget(orbitTarget) {};

    glm::mat4 getModelMatrix();
    void updateOrbitAndRotation();
    void update();

    glm::vec3 scale;
    glm::vec3 position;
    float rotationSpeed;
    float rotationAngle = 0.0f;
    float orbitSpeed;
    float orbitRadius;
    float orbitAngle = 0.0f;
    glm::vec3 orbitTarget;
};