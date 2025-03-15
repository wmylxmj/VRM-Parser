//
// Created by 13973 on 25-3-15.
//

#pragma once

#include "PrecompiledHeader.h"

class Camera {
private:
    glm::vec3 m_rightDirection = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 m_upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_backDirection = glm::vec3(0.0f, 0.0f, 1.0f);

public:
    glm::vec3 eyePosition = glm::vec3(0, 0, 0);
    float fovY = glm::radians(60.0f);
    float aspect = 16.0 / 9;
    float zNear = 0.1f;
    float zFar = 100.0f;

    void RotateXY(float dThetaX, float dThetaY);
    [[nodiscard]] glm::mat4 GetPerspectiveMatrix() const;
    [[nodiscard]] glm::mat4 GetCameraMatrix() const;
    [[nodiscard]] glm::vec3 GetFrontDirection() const;
    [[nodiscard]] glm::vec3 GetUpDirection() const;
    [[nodiscard]] glm::vec3 GetRightDirection() const;
};


