//#include "Geometry.h"
#include "Shader.h"
#include "Camera.h"
#include "Runtimefunctions.h"
#include "HighLevelRendering.h"

#include "ChaosGame.h"

#include "Geometry.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>

#include <iostream>
#include <string>
#include <cmath>
#include <vector>

const double pi = 3.14159265358979323846;

int main() {
    // Init functions
    glfwInitSetup();
    GLFWwindow* window = glfwWindowSetup("OpenGL Test");
    gladInit();
    
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    // check number of vertex attributes
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum vertex attributes supported: " << nrAttributes << "\n";

    // Shaders
    //Shader shaderSimple("Shaders/GeometrySimple.vs","","Shaders/GeometrySimple.fs");
    Shader shaderConst("Shaders/GeometrySimple.vs", "", "Shaders/GeometryConst.fs");
    
    // Shapes
    //Rectangle shape(2, 2, Decomp_type::TRI);
    //RegularPolygon shape(5, 1);
    //Simplex shape(1);
    std::vector<float> shape(createRegularPolygon(7, 2));
    //shape = insertMidpoints_prism(shape);
    //for (size_t i = 0; i < shape.size(); i += 3) {
    //    std::cout << shape[i] << "\t" << shape[i + 1] << "\t" << shape[i + 2] << std::endl;
    //}
    
    // start timing
    float t0 = static_cast<float>(glfwGetTime());

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    // initalize points
    size_t num_points = 1000000;
    float seed[]{ 0,0,0 };
    float* vs = chaos_game_restricted(shape, seed, .4, num_points);

    // send to GPU
    GPUdata points;
    points.sendToGPU(num_points, vs);
    
    // render loop
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float t1 = static_cast<float>(glfwGetTime());
        float dt = t1 - t0;
        t0 = t1;

        processInput(window, dt);

        // render background
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = glm::perspective(glm::radians(CAMERA.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = CAMERA.GetViewMatrix();
        
        //shaderSimple.set();
        //shaderSimple.setUniform_Mat4("projection", projection);
        //shaderSimple.setUniform_Mat4("view", view);

        //shaderSimple.setUniform_Mat4("model", model);
        //shape.draw();

        shaderConst.set();
        shaderConst.setUniform_Mat4("projection", projection);
        shaderConst.setUniform_Mat4("view", view);

        shaderConst.setUniform_Mat4("model", model);

        points.render(num_points);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] vs;
    glfwTerminate();
    return 0;
}
