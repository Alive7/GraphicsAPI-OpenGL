#ifndef RUNTIME_HH
#define RUNTIME_HH

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Camera;

// OpenGL constants
#define SCR_WIDTH 1000
#define SCR_HEIGHT 800

// Global camera declaration
extern Camera CAMERA;
extern float LAST_X;
extern float LAST_Y;
extern bool FIRST_MOUSE;

// initalization functions
void glfwInitSetup(void);
GLFWwindow* glfwWindowSetup(const char* title);
void gladInit(void);

// callback functions
// Should camera related callbacks be in camera?
void processInput(GLFWwindow* window, float dt);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double x_step, double y_step);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif