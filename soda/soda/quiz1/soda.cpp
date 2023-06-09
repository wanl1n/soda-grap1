#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cmath"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <string>
#include <iostream>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float height = 600.0f;
    float width = 600.0f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(height, width, "Kate Nicole Young", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    // Vertex shader for positioning
    std::fstream vertSrc("../Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    // Fragment shader for coloring
    std::fstream fragSrc("../Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    // Creating the vertex shader for use in the program.
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);

    // Creating the fragment shader for use in the program.
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    // Creating the shader program.
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    // Links the shader to the program.
    glLinkProgram(shaderProgram);

    // Bunny Object elements
    std::string path = "../3D/bunny.obj";
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shape,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shape[0].mesh.indices[i].vertex_index
        );
    }

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // We're working with this VAO.
    glBindVertexArray(VAO);
    // We're working with this VBO.
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Binds VBO to VAO.
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * attributes.vertices.size(),
        &attributes.vertices[0],
        GL_STATIC_DRAW //GL_DYNAMIC_DRAW
    );

    // Tells VAO how to interpret above data.
    glVertexAttribPointer(
        0, // Index: 0 == Position, 1 == UV, 2 == Texture.
        3, // XYZ
        GL_FLOAT, // Type of array
        GL_FALSE, // If need normalize, TRUE
        3 * sizeof(GL_FLOAT), // Size of the vertex data
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    // Clean Up
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Wala nang ginagalaw sa VBO.
    glBindVertexArray(0); // Wala ka nang ginagalaw na VAO.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // EBO

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        float radius = 0.5f; 
        glm::vec3 cameraCenter = glm::vec3(0, 0.f, 0);

        glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0f), cameraCenter);
        glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0, 0, 0.5f), cameraCenter, glm::normalize(glm::vec3(0, 1, 0)));
        glm::mat4 projection = glm::perspective(glm::radians(60.f), height / width, 0.1f, 100.f);

        unsigned int rCol = glGetUniformLocation(shaderProgram, "r");
        unsigned int gCol = glGetUniformLocation(shaderProgram, "g");
        unsigned int bCol = glGetUniformLocation(shaderProgram, "b");

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);

        // Top left -- Top View ------------------------------------------------------------
        glViewport(0, height/2, width/2, height/2);

        glUniform1f(rCol, 1.5f);
        glUniform1f(gCol, 0.f);
        glUniform1f(bCol, 1.5f);
        
        // Bunny 1 -- top view
        glm::mat4 topTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        topTransform = glm::rotate(topTransform, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(topTransform));

        // Option 2: Camera View Adjustment
            glm::vec3 topCameraPos = glm::vec3(0, sin(90.f) * radius, cos(90.f) * radius);
            glm::mat4 topViewMatrix = glm::lookAt(topCameraPos, cameraCenter, glm::normalize(glm::vec3(0, 1, 0)));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        
        glUseProgram(shaderProgram);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);


        // Top right -- Side View ----------------------------------------------------------
        glViewport(width / 2, height / 2, width / 2, height / 2); 
        
        glUniform1f(rCol, 1.5f);
        glUniform1f(gCol, 1.5f);
        glUniform1f(bCol, 0.f);

        // Bunny 2 -- side view
        glm::mat4 sideTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        sideTransform = glm::rotate(sideTransform, glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(sideTransform));

        // Option 2: Camera View Adjustment
            glm::vec3 sideCameraPos = glm::vec3(sin(90.f) * radius, 0, cos(90.f) * radius);
            glm::mat4 sideViewMatrix = glm::lookAt(sideCameraPos, cameraCenter, glm::normalize(glm::vec3(0, 1.f, 0)));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        
        glUseProgram(shaderProgram);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);


        // Bottom -- Front View ------------------------------------------------------------
        glViewport(width / 4, 0, width/2, height/2); 
        
        glUniform1f(rCol, 0.f);
        glUniform1f(gCol, 1.5f);
        glUniform1f(bCol, 1.5f);

        // Bunny 3 -- front view
        glm::mat4 frontTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(frontTransform));
       
        // Option 2: Camera View Adjustment
            glm::vec3 frontCameraPos = glm::vec3(0, 0, radius);
            glm::mat4 frontViewMatrix = glm::lookAt(frontCameraPos, cameraCenter, glm::normalize(glm::vec3(0, 1.f, 0)));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
       
        glUseProgram(shaderProgram);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);


        glViewport(0, 0, width, height);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}