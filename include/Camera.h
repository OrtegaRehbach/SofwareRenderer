#pragma once

#include "glm/glm.hpp"

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
    void Rotate(float angleRadians) {
        // Rotate the camera around its own up vector
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians, upVector);
        // Update the target position and right vector
        targetPosition = cameraPosition + glm::mat3(rotationMatrix) * (targetPosition - cameraPosition);
        rightVector = glm::normalize(glm::cross(targetPosition - cameraPosition, upVector));
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