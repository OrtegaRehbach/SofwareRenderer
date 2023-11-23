#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

struct Camera {
    glm::vec3 cameraPosition;
    glm::vec3 targetPosition;
    glm::vec3 upVector;
    glm::vec3 rightVector;
    glm::vec3 viewDirection;

    // Constructor to initialize camera properties
    Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
        : cameraPosition(position), targetPosition(target), upVector(up) {
        // Calculate the initial right vector
        rightVector = glm::normalize(glm::cross(target - cameraPosition, upVector));
        viewDirection = glm::normalize(targetPosition - cameraPosition);
    }

    // Recalculate the camera's viewDirection vector
    void updateViewDirection() {
        viewDirection = glm::normalize(targetPosition - cameraPosition);
    }

    // Function to rotate the camera horizontally (left or right)
    void Rotate(float deltaX, float deltaY) {
        float rotationSpeed = 1.0f;    
        glm::quat quatRotY = glm::angleAxis(glm::radians(deltaX * rotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat quatRotX = glm::angleAxis(glm::radians(deltaY * rotationSpeed), glm::vec3(1.0f, 0.0f, 0.0f));

        cameraPosition = targetPosition + quatRotY * (cameraPosition - targetPosition);
        cameraPosition = targetPosition + quatRotX * (cameraPosition - targetPosition);
        updateViewDirection();
    }

    // Function to move the camera forward
    void MoveForward(float movementSpeed) {
        cameraPosition += viewDirection * movementSpeed;
        targetPosition += viewDirection * movementSpeed;
        updateViewDirection();
    }

    // Function to move the camera backward
    void MoveBackward(float movementSpeed) {
        cameraPosition -= viewDirection * movementSpeed;
        targetPosition -= viewDirection * movementSpeed;
        updateViewDirection();
    }

    void MoveRight(float movementSpeed) {
        cameraPosition += rightVector * movementSpeed;
        targetPosition += rightVector * movementSpeed;
        updateViewDirection();
    }

    void MoveLeft(float movementSpeed) {
        cameraPosition -= rightVector * movementSpeed;
        targetPosition -= rightVector * movementSpeed;
        updateViewDirection();
    }
};