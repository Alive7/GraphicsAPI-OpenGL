#include "Runtimefunctions.h"
#include "Camera.h"

#include <stb/stb_image_comp.h>

#include <iostream>

// Global camera implementation
Camera CAMERA(glm::vec3(0.0f, 0.0f, 10.0f));
float LAST_X = SCR_WIDTH / 2.0f;
float LAST_Y = SCR_HEIGHT / 2.0f;
bool FIRST_MOUSE = true;

// initalization functions
void glfwInitSetup(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 6
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

GLFWwindow* glfwWindowSetup(const char* title) {
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
}

void gladInit(void) {
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        glfwTerminate();
    }
}

// callback functions
// should this be a callback?
void processInput(GLFWwindow* window, float dt) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        CAMERA.ProcessKeyboard(Camera_Movement::FORWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        CAMERA.ProcessKeyboard(Camera_Movement::BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        CAMERA.ProcessKeyboard(Camera_Movement::LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        CAMERA.ProcessKeyboard(Camera_Movement::RIGHT, dt);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (FIRST_MOUSE) {
        LAST_X = xposIn;
        LAST_Y = yposIn;
        FIRST_MOUSE = false;
    }
    float xoffset = xposIn - LAST_X;
    float yoffset = LAST_Y - yposIn; // reversed since y-coordinates go from bottom to top
    LAST_X = xposIn;
    LAST_Y = yposIn;

    CAMERA.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double x_step, double y_step) {
    CAMERA.ProcessMouseScroll(static_cast<float>(y_step));
}
