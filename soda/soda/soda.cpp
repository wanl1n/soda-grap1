#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cmath"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <string>
#include <iostream>

float speed = 0.05f;
float radius = 30.f;
float scale_mod = 0.1f;
float theta_tot = 90.f;

float x_mod = 0;
float y_mod = 0;
float z_mod = 50.f;

bool isMovingLeft = false;
bool isMovingRight = false;
bool isMovingForward = false;
bool isMovingBack = false;

bool isSpawning = false;

class Model3D {
private:
    glm::vec3 pos;
    glm::vec3 rotation; // in degrees
    glm::vec3 scale;
    std::vector<GLuint> mesh_indices;

public:
    Model3D(glm::vec3 pos, std::vector<GLuint> mesh_indices) {
        this->pos = pos;
        this->rotation = glm::vec3(0, 0, 0);
        this->scale = glm::vec3(scale_mod, scale_mod, scale_mod);
        this->mesh_indices = mesh_indices;
    }

    void draw(GLuint* shaderProgram) {
        
        /* * * * * * * * * * * * TRANSFORMING THE OBJECT * * * * * * * * * * * */
        // Create the transformation matrix.
        glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0f), this->pos);
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale_mod));

        unsigned int transformLoc = glGetUniformLocation(*shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        glUseProgram(*shaderProgram);

        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
    }
};

void Key_Callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
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
        isMovingForward = true;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        isMovingForward = false;

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        isMovingBack = true;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        isMovingBack = false;

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        isSpawning = true;
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    float height = 600.0f;
    float width = 600.0f;

    window = glfwCreateWindow(height, width, "Kate Nicole Young", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    int img_width, img_height, color_channels; // Width, Height, and color channels of the Texture.

    stbi_set_flip_vertically_on_load(true);
    // Texture Credits: "Lazy Gudetama" (https://skfb.ly/6u6GT) by Elyse Darby is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
    unsigned char* text_bytes = stbi_load("../3D/gudetama.png", // Texture path
                                          &img_width, // Width of the texture
                                          &img_height, // height of the texture
                                          &color_channels, // color channel
                                          0);
    img_width /= 10;
    img_height /= 10;

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT);

    glTexImage2D(
        GL_TEXTURE_2D,
        0, 
        GL_RGBA,
        img_width,
        img_height,
        0, 
        GL_RGBA,
        GL_UNSIGNED_BYTE, 
        text_bytes
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(text_bytes);

    glEnable(GL_DEPTH_TEST);

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

    // Model Credits: "Lazy Gudetama" (https://skfb.ly/6u6GT) by Elyse Darby is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
    std::string path = "../3D/gudetama.obj";
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

    GLfloat UV[]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
    };

    GLuint VAO, VBO, EBO, VBO_UV;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO_UV);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER, 
        sizeof(GLfloat) * attributes.vertices.size(),
        &attributes.vertices[0],
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(GL_FLOAT),
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

    glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])),
                 &UV[0],
                 GL_DYNAMIC_DRAW
    );
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glm::mat4 projection = glm::perspective(
        glm::radians(60.f),
        height / width,
        0.1f,
        100.f
    );

    // Stores the mouse cursor positions.
    double x_cursor_pos, y_cursor_pos;
    // Stores all the models currently in the world.
    std::vector<Model3D*> models;
    // Spawn 1 model in at the start of the program.
    models.push_back(new Model3D(glm::vec3(0, 0, 0), mesh_indices));
    glfwSetTime(0); // Start the timer.

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        glfwGetCursorPos(window, &x_cursor_pos, &y_cursor_pos);

        if (isMovingLeft) x_mod -= speed;
        if (isMovingRight) x_mod += speed;
        if (isMovingForward) z_mod -= speed;
        if (isMovingBack) z_mod += speed;

        /* * * * * ADJUSTING THE CAMERA ACCORDING TO THE MOUSE POSITION * * * * */
        // Calculate the position of the mouse with the origin (0, 0) at the center of the window.
        glm::vec2 mousePos = glm::vec2(x_cursor_pos - (width / 2), y_cursor_pos - (height / 2));
        // Calculate the new camera center coordinates using Polar Coordinates.
        // Assuming the screen is a number line from -90 to 90 (to represent the degree of rotation from the center/origin),
        // First, use linear interpolation to get the relative distance of the mouse cursor position.
        // Second, multiply it with the new "scale" (-90 to 90) to get the degree.
        // Third, convert it to radians and using the angle, calculate the Cartesian coordinate.
        float xAxisRot = radius * sin(glm::radians((mousePos.x / (width / 2)) * 90.f));
        float yAxisRot = radius * sin(glm::radians((mousePos.y / (height / 2)) * -90.f));
        float zAxisRot = radius * cos(glm::radians((mousePos.x / (width / 2)) * -90.f));
        // Update the camera center (where the camera is looking at) with the new calculated point.
        // Finally, make sure to add the strafing movement of the camera to the x-axis.
        glm::vec3 cameraCenter = glm::vec3(x_mod + xAxisRot, yAxisRot, zAxisRot);

        // Create the view matrix.
        glm::mat4 viewMatrix = glm::lookAt(glm::vec3(x_mod, y_mod, z_mod),
                                            cameraCenter,
                                            glm::vec3(0.0f, 1.0f, 0.f));

        // Updating the view Matrix in the shader program.
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        
        // Updating the projection in the shader program.
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // If the spacebar was clicked,
        if (isSpawning) {

            // First check if 3 seconds have passed since the last spawn.
            if (glfwGetTime() > 3) {
                glfwSetTime(0);
                models.push_back(new Model3D(cameraCenter, mesh_indices));
            }
            
            isSpawning = false;
        }
        
        /* * * * * * * * * * ADDING THE TEXTURE TO THE SHADERS * * * * * * * * * */
        // Optional --!
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        glBindVertexArray(VAO);

        for (Model3D* model : models) {
            model->draw(&shaderProgram);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}