//
// Created by 13973 on 25-3-15.
//

#include "Camera.h"

void Camera::RotateXY(const float dThetaX, const float dThetaY) {
    // Rotate Yaw
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), dThetaX, glm::vec3(0.0f, -1.0f, 0.0f));
    m_upDirection = glm::normalize(rotationMatrix * glm::vec4(m_upDirection, 1.0f));
    m_backDirection = glm::normalize(rotationMatrix * glm::vec4(m_backDirection, 1.0f));
    m_rightDirection = glm::cross(m_upDirection, m_backDirection);

    // Rotate Pitch
    rotationMatrix = glm::rotate(glm::mat4(1.0f), dThetaY, m_rightDirection);
    m_upDirection = glm::normalize(rotationMatrix * glm::vec4(m_upDirection, 1.0f));
    m_backDirection = glm::normalize(rotationMatrix * glm::vec4(m_backDirection, 1.0f));
    m_rightDirection = glm::cross(m_upDirection, m_backDirection);
}

glm::mat4 Camera::GetCameraMatrix() const {
    return glm::lookAt(eyePosition, eyePosition - m_backDirection, m_upDirection);
}

glm::mat4 Camera::GetPerspectiveMatrix() const {
    return glm::perspective(fovY, aspect, zNear, zFar);
}


glm::vec3 Camera::GetFrontDirection() const {
    return - m_backDirection;
}

glm::vec3 Camera::GetUpDirection() const {
    return m_upDirection;
}

glm::vec3 Camera::GetRightDirection() const {
    return m_rightDirection;
}
