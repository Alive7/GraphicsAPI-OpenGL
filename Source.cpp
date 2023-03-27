#include "Geometry.h"
#include "Shader.h"
#include "Texture.h"
#include "CubeMap.h"
#include "Camera.h"
#include "Runtimefunctions.h"

#include "Mesh.h"
#include "Model.h"

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
    
    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    // enable transparancy
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // enable face culling - need to be creful with 2d primatives
    glEnable(GL_CULL_FACE);
    // disable writing to the depth buffer
    //glDepthMask(GL_FALSE);
    // change depth test comparison
    //glDepthFunc(GL_LESS);

    // enable stencil testing
    //glEnable(GL_STENCIL_TEST);
    // disable writing to the stencil buffer
    //glStencilMask(0x00);
    // set options for stencil test
    //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // check number of vertex attributes
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum vertex attributes supported: " << nrAttributes << "\n";

    // Shaders
    //Shader shaderGeo("Shaders/GeometrySimple.vs", "Shaders/passthrough.gs", "Shaders/GeometrySimple.fs");
    Shader shaderGeo("Shaders/GeometryInstanced.vs", "", "Shaders/GeometrySimple.fs");
    Shader shaderBox("Shaders/Skybox.vs", "", "Shaders/Skybox.fs");
    //Shader screenShader("Shaders/GeometryNoCam.vs", "Shaders/GeometryConst.fs");
    //Shader shader("Shaders/GeometryLightingTexture.vs", "Shaders/GeometryLightingTexture.fs");
    //Shader sourceShader("Shaders/GeometrySimple.vs", "Shaders/GeometrySimple.fs");
    //Shader modelShader("Shaders/Model.vs", "Shaders/Model.fs");

    
    // Shapes
    //Sphere shape(.5, 40, 40);
    RectangularPrism shape(.1, .1, .1);
    //RectangularPrism shape(1, 1, 1, Decomp_type::TRI);
    //Rectangle shape(1, 1, Decomp_type::TRI);
    //Triangle shape(1, 1, 1, Triangle_type::SSS);
    //RegularPolygon shape(10, 1);

    size_t instances = 100000;
    shape.initalizeInstancing(instances);

    /*
    for (int i = 0; i < shape.vertices.size(); i+=3) {
        std::cout << "[" << shape.vertices[i] << ", " << shape.vertices[i + 1] << ", " << shape.vertices[i + 2] << "]\t";
        if ((i+2) % 12 == 11) std::cout << std::endl;
    }
    std::cout << std::endl;
    for (int i = 0; i < shape.indices.size(); i+=3) {
        std::cout << "[" << shape.indices[i] << ", " << shape.indices[i + 1] << ", " << shape.indices[i + 2] << "]\t";
        if ((i + 2) % 6 == 5) std::cout << std::endl;
    }
    std::cout << std::endl;
    */

    //Rectangle floor(1, 1, Decomp_type::TRI);
    //Sphere lightSource(.05, 40, 40);

    // draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Textures
    //Texture texture1("container.png");
    //Texture texture2("glass.png");
    //Texture texture2("container_specular.png");
    //Texture texture3("glow_text.jpg");

    // world space positions of cubes
    /*
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    */

    // Shader Initalization    
    shaderGeo.set();
    // Gemonetry Simple
    shaderGeo.setUniform_Vec3("color", glm::vec3(1.0, 0.2, 0.4));
    //shader.setUniform_Int("texture0", texture1.unit);
    /*
    // Geometry Lighting
    // material properties
    //.setUniform_Vec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    //shader.setUniform_Vec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    //shader.setUniform_Vec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    shader.setUniform_Int("material.diffuse", texture1.unit);
    shader.setUniform_Int("material.specular", texture2.unit);
    //shader.setUniform_Int("material.emission", texture3.unit);
    shader.setUniform_Float("material.shininess", 32.0f);

    // directional light
    glm::vec3 dirLight_col(1.0f, 1.0f, 1.0f);
    shader.setUniform_Vec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    shader.setUniform_Vec3("dirLight.ambient", dirLight_col*glm::vec3(0.05f));
    shader.setUniform_Vec3("dirLight.diffuse", dirLight_col*glm::vec3(0.4f));
    shader.setUniform_Vec3("dirLight.specular", dirLight_col*glm::vec3(0.5f));

    // point light
    glm::vec3 pntLight_pos(1.2f, 1.0f, 2.0f);
    //glm::vec3 pntLight_col(0.2f, 0.7f, 1.0f);
    glm::vec3 pntLight_col(1.0f, 1.0f, 1.0f);
    shader.setUniform_Vec3("pos_src_pnt", pntLight_pos);
    shader.setUniform_Vec3("pntLight.ambient", pntLight_col*glm::vec3(0.05f));
    shader.setUniform_Vec3("pntLight.diffuse", pntLight_col*glm::vec3(0.8f));
    shader.setUniform_Vec3("pntLight.specular", pntLight_col*glm::vec3(1.0f));
    shader.setUniform_Float("pntLight.constant", 1.0f);
    shader.setUniform_Float("pntLight.linear", 0.09f);
    shader.setUniform_Float("pntLight.quadratic", 0.032f);

    // spot light
    glm::vec3 sptLight_col(1.0f, 1.0f, 1.0f);
    shader.setUniform_Vec3("sptLight.ambient", sptLight_col*glm::vec3(0.0f));
    shader.setUniform_Vec3("sptLight.diffuse", sptLight_col*glm::vec3(1.0f));
    shader.setUniform_Vec3("sptLight.specular", sptLight_col*glm::vec3(1.0f));
    shader.setUniform_Float("sptLight.constant", 1.0f);
    shader.setUniform_Float("sptLight.linear", 0.09f);
    shader.setUniform_Float("sptLight.quadratic", 0.032f);
    shader.setUniform_Float("sptLight.innerCutOff", glm::cos(glm::radians(12.5f)));
    shader.setUniform_Float("sptLight.outerCutOff", glm::cos(glm::radians(15.0f)));
    */

    //sourceShader.set();
    //sourceShader.setUniform_Vec3("color", pntLight_col);

    //stbi_set_flip_vertically_on_load(true);
    //Model ourModel("Models/backpack.obj");

    /*
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    */

    std::vector<std::string> faces {
        "Textures/skybox/right.jpg",
        "Textures/skybox/left.jpg",
        "Textures/skybox/top.jpg",  
        "Textures/skybox/bottom.jpg",
        "Textures/skybox/front.jpg",
        "Textures/skybox/back.jpg"
    };
    CubeMap skybox(faces);

    // start timing
    float t0 = static_cast<float>(glfwGetTime());

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    std::vector<glm::mat4> modelMatrices(instances);
    srand(glfwGetTime());
    float radius = 25.0;
    float offset = 2.5f;
    for (size_t i = 0; i < instances; i++) {
        model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)instances * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

    model = glm::mat4(1.0f);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float t1 = static_cast<float>(glfwGetTime());
        float dt = t1 - t0;
        t0 = t1;

        processInput(window, dt);

        for (size_t i = 0; i < instances; i++) {
            float rotAngle = 2 * pi * dt / ((float)(i%100) + 1 / 10);
            modelMatrices[i] = glm::rotate(modelMatrices[i], rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        }
        
        shape.sendInstancedData(modelMatrices);

        /*
        // first pass
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
        glEnable(GL_DEPTH_TEST);

        glm::mat4 projection = glm::perspective(glm::radians(CAMERA.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = CAMERA.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        shader.set();
        shader.setUniform_Mat4("projection", projection);
        shader.setUniform_Mat4("view", view);
        shader.setUniform_Mat4("model", model);
        shape.draw();

        // second pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.set();
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        floor.draw();
        */

        // render background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = glm::perspective(glm::radians(CAMERA.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = CAMERA.GetViewMatrix();
        model = glm::mat4(1.0f);

        //shader.setUniform_Vec4("color", glm::vec4(0.8,0.4,0.1,1.0));

        //shader.setUniform_Vec3("pos_src_spt", CAMERA.Position);
        //shader.setUniform_Vec3("spotLight.direction", -CAMERA.Z_axis);
        //shader.setUniform_Vec3("sptLight.direction", glm::vec3(0,0,-1));
        
        shaderGeo.set();
        shaderGeo.setUniform_Mat4("projection", projection);
        shaderGeo.setUniform_Mat4("view", view);

        //texture1.bind();
        //texture2.bind();
        //texture3.bind();

        //shader.setUniform_Int("texture0", texture1.unit);

        // make sure we don't update the stencil buffer while drawing the floor
        //glStencilMask(0x00);
        //model = glm::mat4(1.0f);
        //model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
        //model = glm::rotate(model, glm::radians((float)90), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.51f));
        //shader.setUniform_Mat4("model", model);
        //floor.draw();


        //texture2.bind();
        //shader.setUniform_Int("texture0", texture2.unit);
        model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)(2 * pi * t1 / 5000), glm::vec3(0.0f, 1.0f, 0.0f));
        shaderGeo.setUniform_Mat4("model", model);
        shape.drawInstanced(instances);
        /*
        for (int i = 0; i < instances; i++) {
            //model = glm::mat4(1.0f);
            //model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
            shaderGeo.setUniform_Mat4("model", modelMatrices[i]);
            //shape.drawInstanced(instances);
            shape.draw();
        }
        */

        //glDisable(GL_CULL_FACE);

        /*
        for (size_t i = 0; i < 10; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i + 20.0f;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setUniform_Mat4("model", model);
            shader.setUniform_Mat3("norm", glm::mat3(glm::transpose(glm::inverse(view * model))));
            shape.draw();
        }
        */
        
        //sourceShader.set();
        //sourceShader.setUniform_Mat4("projection", projection);
        //sourceShader.setUniform_Mat4("view", view);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, pntLight_pos);
        //sourceShader.setUniform_Mat4("model", model);
        //lightSource.draw();
        
        /*
        modelShader.set();
        modelShader.setUniform_Mat4("projection", projection);
        modelShader.setUniform_Mat4("view", view);

        // render the loaded model
        model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        //model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        modelShader.setUniform_Mat4("model", model);
        ourModel.Draw(modelShader);
        */

        shaderBox.set();
        shaderBox.setUniform_Mat4("projection", projection);
        shaderBox.setUniform_Mat4("view", glm::mat4(glm::mat3(view)));

        // rmeove awkwardness by passing textures and cubemaps and possibly colors to draw function?
        
        glBindVertexArray(shape.VAO);
        skybox.bind();
        glCullFace(GL_FRONT);
        glDepthFunc(GL_LEQUAL);
        shape.draw();
        glDepthFunc(GL_LESS);
        glCullFace(GL_BACK);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
