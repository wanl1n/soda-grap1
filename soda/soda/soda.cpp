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

float speed = 0.01f;
float radius = 30.f;
float scale_mod = 0.1f;
float theta_tot = 90.f;

float x_mod = 0;
float y_mod = 0;
float z_mod = -50.f;

bool isMovingLeft = false;
bool isMovingRight = false;
bool isMovingForward = false;
bool isMovingBack = false;

bool isSpawning = false;

class Model3D {
private:
    glm::vec3 pos;
    glm::vec3 rotation; // axis of rotation
    float theta;// in degrees
    glm::vec3 scale;
    std::vector<GLuint> mesh_indices;

public:
    Model3D(glm::vec3 pos, std::vector<GLuint> mesh_indices) {
        this->pos = pos;
        this->rotation = glm::vec3(0, 1, 0);
        this->theta = 180.f;
        this->scale = glm::vec3(scale_mod, scale_mod, scale_mod);
        this->mesh_indices = mesh_indices;
    }

    void draw(GLuint* shaderProgram, std::vector<GLfloat> fullVertexData) {
        
        /* * * * * * * * * * * * TRANSFORMING THE OBJECT * * * * * * * * * * * */
        // Create the transformation matrix.
        glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0f), this->pos);
        transformation_matrix = glm::scale(transformation_matrix, this->scale);
        transformation_matrix = glm::rotate(transformation_matrix,
                                glm::radians(this->theta),
                                this->rotation);

        unsigned int transformLoc = glGetUniformLocation(*shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        glUseProgram(*shaderProgram);

        //glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 5);
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
    // "Puss in banana suit 3D model" (https://skfb.ly/oF7Ay) by Wnight is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
    unsigned char* text_bytes = stbi_load("../3D/BananaCat_vertexcolor.png", // Texture path
                                          &img_width, // Width of the texture
                                          &img_height, // height of the texture
                                          &color_channels, // color channel
                                          0);
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

    // "Puss in banana suit 3D model" (https://skfb.ly/oF7Ay) by Wnight is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
    std::string path = "../3D/banancat.obj";
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

    // Loading the UV data of the object.
    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shape[0].mesh.indices[i];

        // This will generate an array with 3 consecutive points 
        // as a position coordinate ordered in the vector.
        // X
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);
        // Y
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);
        // Z
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);

        // Add normals here
        //fullVertexData.push_back(attributes.vertices[vData.normal_index * 3]);

        // UV
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);
        
        // Result: 3 Pos, 2 UV, 3 Pos, 2 UV, and so on.
    }

    GLuint VAO, VBO;
        //EBO, VBO_UV;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &VBO_UV);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER, 
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        //XYZ UV (change from 3 to 5)
        5 * sizeof(GL_FLOAT),
        (void*)0
    );
    
    GLintptr uvPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GLfloat),
        (void*)uvPtr
    );
    
    glEnableVertexAttribArray(0);

    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );*/

    /*glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
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
    );*/

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

    glm::vec3 cameraPos = glm::vec3(x_mod, y_mod, z_mod);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // Get the position of the cursor in the window.
        glfwGetCursorPos(window, &x_cursor_pos, &y_cursor_pos);

        // Update the new x and y coordinates based on input.
        /*if (isMovingLeft) x_mod -= speed;
        if (isMovingRight) x_mod += speed;*/
        //if (isMovingForward) z_mod -= speed;
        //if (isMovingBack) z_mod += speed;

        /* * * * * ADJUSTING THE CAMERA ACCORDING TO THE MOUSE POSITION * * * * */

        // Calculate the position of the mouse with the origin (0, 0) at the center of the window.
        glm::vec2 mousePos = glm::vec2(x_cursor_pos - (width / 2), y_cursor_pos - (height / 2));

        // Calculating the degree of rotation around the y-axis(yaw) and the x-axis (pitch).
        // Calculate the new camera center coordinates using Polar Coordinates.
        // Assuming the screen is a number line from -90 to 90 (to represent the degree of rotation from the center/origin),
        // First, use linear interpolation to get the relative distance of the mouse cursor position.
        // Second, multiply it with the new "scale" (-90 to 90) to get the degree.
        // Third, convert it to radians.
        float yaw = glm::radians((mousePos.x / (width / 2)) * theta_tot);
        float pitch = glm::radians((mousePos.y / (height / 2)) * -1 * theta_tot);

        // Limiting the degree in case of flipping.
        if (yaw > 90.f) yaw = 90.f;
        if (pitch > 90.f) pitch = 90.f;

        // Finally get the direction in each axis by using Polar to Cartesian point conversion.
        float xAxisRot = radius * sin(yaw) * cos(pitch);
        float yAxisRot = radius * sin(pitch);
        float zAxisRot = radius * cos(yaw) * cos(pitch);

        // Update the camera center with the new calculated point.
        // Finally, make sure to add the strafing movement of the camera to the x-axis.
        glm::vec3 cameraCenter = glm::vec3(xAxisRot, yAxisRot, zAxisRot);

        // Next, calculate the position change based on where the camera center is.
        glm::vec3 worldUp = glm::normalize(glm::vec3(0, 1.f, 0));

        if (isMovingLeft) cameraPos -= glm::normalize(glm::cross(cameraCenter, worldUp)) * speed;
        if (isMovingRight) cameraPos += glm::normalize(glm::cross(cameraCenter, worldUp)) * speed;
        if (isMovingForward) cameraPos += speed * (cameraCenter);
        if (isMovingBack) cameraPos -= speed * (cameraCenter);

        //float xMoveDir = 0, yMoveDir = 0, zMoveDir = 0;        
        //if (isMovingLeft || isMovingRight) {

        //    if (isMovingLeft)
        //    {
        //        xMoveDir = speed * sin(glm::radians((mousePos.x / (width / 2)) * theta_tot - 90.f));
        //        //yMoveDir = speed * sin(glm::radians((mousePos.y / (height / 2)) * -1 * theta_tot - 90.f));
        //        zMoveDir = speed * cos(glm::radians((mousePos.x / (width / 2)) * theta_tot - 90.f)) * -1;
        //    }
        //    if (isMovingRight)
        //    {
        //        xMoveDir = speed * sin(glm::radians((mousePos.x / (width / 2)) * theta_tot + 90.f));
        //        //yMoveDir = speed * sin(glm::radians((mousePos.y / (height / 2)) * theta_tot + 90.f));
        //        zMoveDir = speed * cos(glm::radians((mousePos.x / (width / 2)) * theta_tot + 90.f));
        //    }
        //}

        //if (isMovingForward || isMovingBack) {

        //    xMoveDir += speed * sin(glm::radians((mousePos.x / (width / 2)) * theta_tot));

        //    if (isMovingForward)
        //    {
        //        yMoveDir += speed * sin(glm::radians((mousePos.y / (height / 2)) * -1 * theta_tot));
        //        zMoveDir += speed * cos(glm::radians((mousePos.x / (width / 2)) * theta_tot)) * -1;
        //    }
        //    if (isMovingBack)
        //    {
        //        yMoveDir += speed * sin(glm::radians((mousePos.y / (height / 2)) * theta_tot));
        //        zMoveDir += speed * cos(glm::radians((mousePos.x / (width / 2)) * theta_tot));
        //    }
        //}
        //
        //glm::vec3 cameraPos = glm::vec3(x_mod += xMoveDir, y_mod += yMoveDir, z_mod += zMoveDir);

        
        // Create the view matrix.
        glm::mat4 viewMatrix = glm::lookAt(cameraPos,
                                            cameraCenter,
                                            worldUp);

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
                glfwSetTime(0); // Reset the timer.
                // Add a new model to be rendered at the point the camera is currently looking at.
                models.push_back(new Model3D(cameraCenter, mesh_indices));
            }
            
            isSpawning = false; // Reset the flag.
        }
        
        /* * * * * * * * * * ADDING THE TEXTURE TO THE SHADERS * * * * * * * * * */
        // Optional : Wanted to do this but might not be able to before due date. --! 
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        glBindVertexArray(VAO);

        for (Model3D* model : models) {
            model->draw(&shaderProgram, fullVertexData);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}