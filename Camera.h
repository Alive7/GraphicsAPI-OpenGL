#ifndef CAMERA_HH
#define CAMERA_HH

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

// Default camera values
#define YAW -90.0
#define PITCH 0.0
#define SPEED 5
#define SENSITIVITY 0.1
#define ZOOM 45.0

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
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    //Camera(void);
    Camera(glm::vec3 = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 = glm::vec3(0.0f, 1.0f, 0.0f), float = YAW, float = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    glm::mat4 GetViewMatrix(void);
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean = true);
    void ProcessMouseScroll(float yoffset);

private:
    void updateCameraVectors(void);
};

#endif