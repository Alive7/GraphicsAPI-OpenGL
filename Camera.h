#ifndef CAMERA_HH
#define CAMERA_HH

#include <glm/glm/glm.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 X_axis;
    glm::vec3 Y_axis;
    glm::vec3 Z_axis;
    // euler Angles
    // orientation about y
    float Yaw;
    // orientation about x
    float Pitch;
    // orientation about z
    float Roll;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(glm::vec3 /*position*/ = glm::vec3(0.0f, 0.0f, 3.0f));
    glm::mat4 GetViewMatrix(void);
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessMouseScroll(float y_step);

private:
    void updateCameraVectors(void);
};

#endif