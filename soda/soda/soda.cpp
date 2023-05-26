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

float speed = 0.05f;
float x_mod = 0;
float y_mod = 0;
float r_mod = 0;
float g_mod = 0;
float b_mod = 0;
float rot_mod = 60.f;

bool isMovingUp = false;
bool isMovingDown = false;
bool isMovingLeft = false;
bool isMovingRight = false;
bool isRotating = false;

void Key_Callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action, // Press or Release
    int mod
) {
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        isMovingRight = true;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        isMovingRight = false;

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        isMovingLeft = true;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        isMovingLeft = false;

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        isMovingUp = true;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        isMovingUp = false;

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        isMovingDown = true;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        isMovingDown = false;
    
    if (key == GLFW_KEY_F &&
        action == GLFW_PRESS)
        r_mod -= 1.f;
    if (key == GLFW_KEY_G &&
        action == GLFW_PRESS)
        g_mod -= 1.f;
    if (key == GLFW_KEY_H &&
        action == GLFW_PRESS)
        b_mod -= 1.f;
    if (key == GLFW_KEY_F &&
        action == GLFW_RELEASE)
        r_mod += 1.f;
    if (key == GLFW_KEY_G &&
        action == GLFW_RELEASE)
        g_mod += 1.f;
    if (key == GLFW_KEY_H &&
        action == GLFW_RELEASE)
        b_mod += 1.f;

    if (key == GLFW_KEY_SPACE &&
        action == GLFW_PRESS)
        isRotating = true;
    if (key == GLFW_KEY_SPACE &&
        action == GLFW_RELEASE)
        isRotating = false;
}

int main(void)
{
    GLFWwindow* window;
    // Center of window is 0,0
    // Sides are 1 or -1.

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Kate Nicole Young", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSetKeyCallback(window, Key_Callback);

    std::fstream vertSrc("../Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("../Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

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

    GLfloat vertices[]{
      // x,    y,    z
        0.0f, 0.5f, 0.0f, // Vertex 0
        -0.5f, -0.5f, 0.f,// Vertex 1
        0.5f, -0.5f, 0.f  // Vertex 2
    };

    GLuint indices[]{
        0, 1, 2
    };

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
        /*sizeof(vertices), 
        vertices, */
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
        //sizeof(indices), 
        //indices,
        GL_STATIC_DRAW
    );

    // Clean Up
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Wala nang ginagalaw sa VBO.
    glBindVertexArray(0); // Wala ka nang ginagalaw na VAO.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // EBO

    // Creating identity matrices
    glm::mat3 identity_matrix3 = glm::mat3(1.0f);
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);

    //// Create a 3D translation matrix
    //glm::mat4 translation = glm::translate(identity_matrix4, 
    //                                       glm::vec3(x, y, z));
    //glm::mat4 scale = glm::scale(identity_matrix4,
    //                             glm::vec3(x, y, z));
    //glm::mat4 rotation = glm::rotate(identity_matrix4,
    //                                 glm::radians(theta),
    //                                 glm::vec3(x, y, z));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        unsigned int rCol = glGetUniformLocation(shaderProgram, "r");
        glUniform1f(rCol, r_mod);
        unsigned int gCol = glGetUniformLocation(shaderProgram, "g");
        glUniform1f(gCol, g_mod);
        unsigned int bCol = glGetUniformLocation(shaderProgram, "b");
        glUniform1f(bCol, b_mod);

        glm::mat4 transformation_matrix = glm::translate(identity_matrix4,
            glm::vec3(x_mod, y_mod, 0.f));

        transformation_matrix = glm::scale(transformation_matrix,
            glm::vec3(1.f, 2.f, 1.f));

        // Update
        if (isMovingUp)
            y_mod += speed;
        if (isMovingDown)
            y_mod -= speed;
        if (isMovingLeft)
            x_mod -= speed;
        if (isMovingRight)
            x_mod += speed;
        
        if (isRotating)
            rot_mod += 5.f;

        transformation_matrix = glm::rotate(transformation_matrix,
            glm::radians(rot_mod),
            glm::vec3(0.f, 1.f, 0.f));

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

        glUniformMatrix4fv(
            transformLoc,
            1,
            GL_FALSE,
            glm::value_ptr(transformation_matrix)
        );

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shaderProgram);

        glDrawElements(
            GL_TRIANGLES,
            //sizeof(indices),
            mesh_indices.size(),
            GL_UNSIGNED_INT,
            0
        );

        /*float length = 0.3f;
        float angle, x, y;

        glBegin(GL_POLYGON);
        glVertex2f(length, 0.f);

        for (int i = 1; i <= 4; i++) {
            angle = (72 * i) * (3.1416 / 180);
            x = length * cos(angle);
            y = length * sin(angle);
            glVertex2f(x, y);
        }

        glEnd();*/

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