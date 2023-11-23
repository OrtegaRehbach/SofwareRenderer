#pragma once

#include <iostream>
#include "Color.h"
#include <glm/glm.hpp> // Include the GLM library

struct Fragment
{
    float x;
    float y;
    float z;
    Color color;
    float intensity;
    glm::vec3 worldPosition;
    glm::vec3 originalPosition;

    // Constructor
    Fragment(float _x, float _y, float _z = 0.0f, Color _color = Color(255, 255, 255), float _intensity = 1.0f, const glm::vec3& _worldPosition = glm::vec3(0, 0, 0), const glm::vec3& _originalPosition = glm::vec3(0, 0, 0)) 
        : x(_x), y(_y), z(_z), color(_color), intensity(_intensity), worldPosition(_worldPosition), originalPosition(_originalPosition) {}

    Fragment(glm::vec3 position, Color _color = Color(255, 255, 255), float _intensity = 1.0f, const glm::vec3& _worldPosition = glm::vec3(0, 0, 0), const glm::vec3& _originalPosition = glm::vec3(0, 0, 0)) 
        : x(position.x), y(position.y), z(position.z), color(_color), intensity(_intensity), worldPosition(_worldPosition), originalPosition(_originalPosition) {}

    // Overload the << operator for printing a Fragment
    friend std::ostream& operator<<(std::ostream& os, const Fragment& fragment) {
        os << "Fragment(" << fragment.x << ", " << fragment.y << ", " << fragment.z << ") " << fragment.color << " Intensity: " << fragment.intensity;
        return os;
    }
};
