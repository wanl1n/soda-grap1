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

float speed = 0.01f; // Movement speed of the camera.
float radius = 30.f; // Distance of the spawned object to the camera.=
float theta_tot = -90.f; // Max angle on the left or bottom side of the screen.

// Initial Position of the Camera
float x_mod = 0;
float y_mod = 0;
float z_mod = -50.f;

// Movement flags. Updated by input.
bool isMovingLeft = false;
bool isMovingRight = false;
bool isMovingForward = false;
bool isMovingBack = false;

// Spawn flag. Updated by input.
bool isSpawning = false;

// Create a 3D Model Object.
class Model3D {

    // Worldly attributes of the 3D model (transformations and indices)
    private:
        glm::vec3 pos; // position of spawned model.
        glm::vec3 rotation; // axis of rotation
        float theta; // in degrees
        glm::vec3 scale; // scale of the spawned model.

    // Constructor and draw Function.
    public:
        // Initialize all attributes of an instance.
        Model3D(glm::vec3 pos) {
            this->pos = pos; // Take parameter as position.

            // Since model is initially flipped (facing backwards), flip the model.
            this->rotation = glm::vec3(0, 1, 0); // Set axis of rotation to y-axis.
            this->theta = 180.f; // Rotate around y-axis by 180 degrees to flip the model.

            // Since model is too big, scale it down to 0.1f.
            this->scale = glm::vec3(0.1f, 0.1f, 0.1f);
        }

        void draw(GLuint* shaderProgram, std::vector<GLfloat> fullVertexData) {
            // Create the transformation matrix and apply the transformation attributes at draw.
            glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0f), this->pos);
            transformation_matrix = glm::scale(transformation_matrix, this->scale);
            transformation_matrix = glm::rotate(transformation_matrix,
                                    glm::radians(this->theta),
                                    this->rotation);

            // Update the shader with the new transform of the object.
            unsigned int transformLoc = glGetUniformLocation(*shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

            // Get the shaderprogram to be used.
            glUseProgram(*shaderProgram);
            // Draw the object (XYZUV in arrays) with texture.
            glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 5);
        }
};

// Process keyboard inputs.
void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mod) {
    // If pressed D, set moving right to true.
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        isMovingRight = true;
    // If released D, set moving right to false.
    // The same applies for the A, W, and S.
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

    // If pressed Spacebar, trigger a spawn.
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        isSpawning = true;
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    // Height and width of the window.
    float height = 600.0f;
    float width = 600.0f;

    // Creates window and ends program if it fails.
    window = glfwCreateWindow(height, width, "Kate Nicole Young", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    // Width, height, and color channels of a texture.
    int img_width, img_height, color_channels; // Width, Height, and color channels of the Texture.

    // Flip the texture (because it's flipped by default).
    stbi_set_flip_vertically_on_load(true);
    // "Puss in banana suit 3D model" (https://skfb.ly/oF7Ay) by Wnight is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
    unsigned char* text_bytes = stbi_load("../3D/BananaCat_vertexcolor.png", // Texture path
                                          &img_width, // Width of the texture
                                          &img_height, // height of the texture
                                          &color_channels, // color channel
                                          0);
    // Create a texture.
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // If the texture is too small, just clamp it to the edge.
    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE);

    // Set the texture attributes using the width, height, and texture bytes taken from the loaded texture.
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

    // Generate mipmaps/ smaller versions of the texture for faster rendering at further distances.
    glGenerateMipmap(GL_TEXTURE_2D);
    // Free the memmory.
    stbi_image_free(text_bytes);

    // Enable depth test to make sure the model has depth.
    glEnable(GL_DEPTH_TEST);

    // Check for inputs of the user.
    glfwSetKeyCallback(window, Key_Callback);

    // Shader program setup:
    // Load the vertex shader from a source file.
    std::fstream vertSrc("../Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    // Load the fragment shader from a source file.
    std::fstream fragSrc("../Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    // Create a shader from the laoded vertex shader and compile it in the program.
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);

    // Create a shader from the loaded fragment shader and compile it in the program.
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    // Create a program where both fragment and vertex shaders will be attached.
    // This will be used later when using the shaders.
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    // Link the shader program to the program.
    glLinkProgram(shaderProgram);

    // Load the 3D model obj file.
    // "Puss in banana suit 3D model" (https://skfb.ly/oF7Ay) by Wnight is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
    std::string path = "../3D/banancat.obj";
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    tinyobj::attrib_t attributes;

    // Set its attributes from the obj file.
    bool success = tinyobj::LoadObj(
        &attributes,
        &shape,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    // Loading the Position and UV data of the object.
    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shape[0].mesh.indices[i];
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);
        
    }

    // Generating buffers.
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Insert the model vertices (including UV) into the VBO for rendering).
    glBufferData(
        GL_ARRAY_BUFFER, 
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        GL_STATIC_DRAW
    );
    // Tell it how to interpret the aboce data.
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GL_FLOAT),
        (void*)0
    );
    
    // Create a UV pointer (3 because its at index 3).
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

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

    // Set the projection view to perspective view.
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
    models.push_back(new Model3D(glm::vec3(0, 0, 0)));
    glfwSetTime(0); // Start the timer.

    // Initialize the camera position.
    glm::vec3 cameraPos = glm::vec3(x_mod, y_mod, z_mod);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // Get the position of the cursor in the window.
        glfwGetCursorPos(window, &x_cursor_pos, &y_cursor_pos);

        /* * * * * ADJUSTING THE CAMERA ACCORDING TO THE MOUSE POSITION * * * * */

        // Calculate the position of the mouse with the origin (0, 0) at the center of the window.
        glm::vec2 mousePos = glm::vec2(x_cursor_pos - (width / 2), y_cursor_pos - (height / 2));

        // Calculate the new camera center coordinates using Polar Coordinates.
        // Calculating the degree of rotation around the y-axis(yaw) and the x-axis (pitch).
        // Assuming the screen is a number line from -90 to 90 (to represent the degree of rotation from the center/origin),
        // First, use linear interpolation to get the relative distance of the mouse cursor position.
        // Second, multiply it with the new "scale" (-90 to 90) to get the degree.
        // Third, convert it to radians.
        float yaw = glm::radians((mousePos.x / (width / 2)) * theta_tot);
        float pitch = glm::radians((mousePos.y / (height / 2)) * theta_tot);

        // Limiting the degree in case of flipping.
        if (yaw > 89.9f) yaw = 89.9f;
        if (yaw < -89.9f) yaw = -89.9f;
        if (pitch > 89.9f) pitch = 89.9f;
        if (pitch < -89.9f) pitch = -89.9f;

        // Finally get the direction in each axis by using Polar to Cartesian point conversion.
        float xAxisRot = radius * sin(yaw) * cos(pitch);
        float yAxisRot = radius * sin(pitch);
        float zAxisRot = radius * cos(yaw) * cos(pitch);

        // Update the camera center with the new calculated point.
        // Finally, make sure to add the strafing movement of the camera to the x-axis.
        glm::vec3 cameraCenter = glm::vec3(xAxisRot, yAxisRot, zAxisRot);

        // Next, calculate the position change based on where the camera center is.
        glm::vec3 worldUp = glm::normalize(glm::vec3(0, 1.f, 0));
        
        // If moving sideways, add or subtract the normalized right vector of the camera to move the camera position sideways.
        if (isMovingLeft) cameraPos -= glm::normalize(glm::cross(cameraCenter, worldUp)) * 0.5f;
        if (isMovingRight) cameraPos += glm::normalize(glm::cross(cameraCenter, worldUp)) * 0.5f;
        // If moving forward or back, go towards or away from the camera center.
        if (isMovingForward) cameraPos += speed * (cameraCenter);
        if (isMovingBack) cameraPos -= speed * (cameraCenter);

        // Create the view matrix.
        glm::mat4 viewMatrix = glm::lookAt(cameraPos,
                                           cameraPos+ cameraCenter, // to make sure cameracenter is always infront of camera pos.
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
                models.push_back(new Model3D(cameraPos + cameraCenter));
            }
            
            isSpawning = false; // Reset the flag.
        }
        
        /* * * * * * * * * * ADDING THE TEXTURE TO THE SHADERS * * * * * * * * * */
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        glBindVertexArray(VAO);

        // Drawing all existing models.
        for (Model3D* model : models) {
            model->draw(&shaderProgram, fullVertexData);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}