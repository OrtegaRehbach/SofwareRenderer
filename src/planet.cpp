#include "planet.h"


void Planet::updateOrbitAndRotation() {
    if (orbitSpeed != 0) {
        orbitAngle += orbitSpeed;
        float xPos = orbitTarget.x + orbitRadius * std::cos(orbitAngle);
        float zPos = orbitTarget.z + orbitRadius * std::sin(orbitAngle);
        position = glm::vec3(xPos, position.y + orbitTarget.y, zPos);
    }
    rotationAngle += rotationSpeed;
}

void Planet::update() {
    updateOrbitAndRotation();
}

glm::mat4 Planet::getModelMatrix() {
    return createModelMatrix(scale, position, rotationAngle);
}