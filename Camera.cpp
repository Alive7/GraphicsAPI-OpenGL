#include "Camera.h"

#include <glm/glm/gtc/matrix_transform.hpp>

const glm::vec3 World_Y(0.0f, 1.0f, 0.0f);

Camera::Camera(glm::vec3 position) :
    Position{position},
    Yaw{0.0},
    Pitch{0.0},
    Roll{0.0},
    Z_axis{ glm::vec3(0.0f, 0.0f, 1.0f) },
    MovementSpeed{ 5 }, 
    MouseSensitivity{ 0.1 },
    Zoom{ 45.0 }
{
    X_axis = glm::cross(World_Y, Z_axis);
    Y_axis = glm::cross(Z_axis, X_axis);
}

glm::mat4 Camera::GetViewMatrix(void) {
    return glm::lookAt(Position, Position - Z_axis, Y_axis);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD)
        Position -= Z_axis * velocity;
    if (direction == Camera_Movement::BACKWARD)
        Position += Z_axis * velocity;
    if (direction == Camera_Movement::LEFT)
        Position -= X_axis * velocity;
    if (direction == Camera_Movement::RIGHT)
        Position += X_axis * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    Yaw -= xoffset * MouseSensitivity;
    Pitch += yoffset * MouseSensitivity;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    // should be able to replace with a dot product
    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float y_step) {
    Zoom -= y_step;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors(void) {
    glm::vec3 newZ;
    // transform obtained by roation matrix Ry(yaw)Rx(pitch) acting on z axis
    // yaw and pitch probably do not correspond to normal definitions with this scheme
    // this coordinate rotation should be able to give the new x,y,z axes directly
    // should also be able to generalize transform to include roll
    newZ.x = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    newZ.y = -sin(glm::radians(Pitch));
    newZ.z = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Z_axis = glm::normalize(newZ);
    X_axis = glm::normalize(glm::cross(World_Y, Z_axis));
    Y_axis = glm::normalize(glm::cross(Z_axis, X_axis));
}
