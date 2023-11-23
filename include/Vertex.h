#pragma once

#include <iostream>
#include <glm/glm.hpp> // Include the GLM library

struct Vertex {
    glm::vec3 position; // Using glm::vec3 for x, y, z coordinates
    glm::vec3 normal;   // Normal vector
    glm::vec3 originalPos;

    // Empty constructor
    Vertex() : position(glm::vec3(0, 0, 0)), normal(glm::vec3(0, 1, 0)), originalPos(this->position) {}

    // Constructor
    Vertex(const glm::vec3& pos, const glm::vec3& norm)
    : position(pos), normal(norm), originalPos(this->position) {}

    Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec3& ogPos)
    : position(pos), normal(norm), originalPos(ogPos) {}

    // Overload the << operator for printing a Vertex
    friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex) {
        os << "Vertex(" << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z
        << "), Normal(" << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z
        // << "), OgPos(" << vertex.originalPos.x << ", " << vertex.originalPos.y << ", " << vertex.originalPos.z
        << ")";
        return os;
    }
};