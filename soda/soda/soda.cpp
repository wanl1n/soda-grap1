// All documentations for seperate classes are in the .cpp file.

// Define the libraries once.
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

// Contains all the external library includes
#include "soda.h"

// All the includes for the user-defined classes.
#include "Camera/MyCamera.h"
#include "Camera/OrthoCamera.h"
#include "Camera/PerspectiveCamera.h"

#include "Light/Light.h"
#include "Light/DirectionLight.h"
#include "Light/PointLight.h"

#include "Model/Model.h"
#include "Model/TexturedModel.h"
#include "Model/LightSource.h"

// namespaces of the user-defined classes.
using namespace cameras;
using namespace lighting;
using namespace models;

// Common variables used all throughout the main loop.
float speed = 0.1f;
glm::vec4 green = glm::vec4(0, 1, 0, 1);
glm::vec4 white = glm::vec4(1, 1, 1, 1);

// Event flags for inputs.
bool changeToPers = false;
bool changeToOrtho = false;
bool changeSelected = false;

bool isRotatingUp = false;
bool isRotatingDown = false;
bool isRotatingRight = false;
bool isRotatingLeft = false;
bool isRotatingCounter = false;
bool isRotatingClockwise = false;

bool pointIncIntensity = false;
bool pointDecIntensity = false;
bool dirIncIntensity = false;
bool dirDecIntensity = false;

bool allowFreeMovement = false;

// Input checker
void Key_Callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action, // Press or Release
    int mod
) {
    std::cout << "Pressed Key: " << key << std::endl;

    // Changing Camera
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        changeToPers = true;
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        changeToOrtho = true;

    // Changing Selection of Model
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        changeSelected = true;

    // Rotate WS = X-axis, AD = Y-axis, QE = Z-axis
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        isRotatingRight = true;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        isRotatingRight = false;
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        isRotatingUp = true;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        isRotatingUp = false;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        isRotatingDown = true;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        isRotatingDown = false;
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        isRotatingLeft = true;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        isRotatingLeft = false;
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        isRotatingCounter = true;
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
        isRotatingCounter = false;
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        isRotatingClockwise = true;
    if (key == GLFW_KEY_E && action == GLFW_RELEASE)
        isRotatingClockwise = false;

    // Adjusting the intensity of the point light
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        pointIncIntensity = true;
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
        pointIncIntensity = false;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        pointDecIntensity = true;
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
        pointDecIntensity = false;

    // Adjusting the intensity of the direction light
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        dirIncIntensity = true;
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        dirIncIntensity = false;
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        dirDecIntensity = true;
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        dirDecIntensity = false;

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        allowFreeMovement = !allowFreeMovement;
}

void CreateProgram(const char* pathVert, const char* pathFrag, GLuint* shaderProgram) {
    // Vertex shader for positioning
    std::fstream vertSrc(pathVert);
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    // Fragment shader for coloring
    std::fstream fragSrc(pathFrag);
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
    *shaderProgram = glCreateProgram();
    glAttachShader(*shaderProgram, vertShader);
    glAttachShader(*shaderProgram, fragShader);

    // Links the shader to the program.
    glLinkProgram(*shaderProgram);

    // Clean up
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

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

    glEnable(GL_DEPTH_TEST);

    // Calls the input getter.
    glfwSetKeyCallback(window, Key_Callback);

    // Creates the shader programs for the models
    GLuint shaderProgram, lightShaderProgram;
    CreateProgram("Shaders/sample.vert", "Shaders/sample.frag", &shaderProgram);
    CreateProgram("Shaders/lightsource.vert", "Shaders/lightsource.frag", &lightShaderProgram);

    // Cameras
    OrthoCamera orthoCamera = OrthoCamera(height, width);
    PerspectiveCamera persCamera = PerspectiveCamera(height, width);
    MyCamera* mainCamera = &persCamera;

    // Direciton Light
    DirectionLight dirLight = DirectionLight(glm::vec3(4, 11, -3));

    // Model Data
    // "Sanrio Cinnamoroll" (https://skfb.ly/oENJM) by woomie is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
    TexturedModel primaryModel("3D/cinna.obj", glm::vec3(0, 0, 0), glm::vec3(5.f, 5.f, 5.f), &shaderProgram,
                                "3D/cinna.png", GL_TEXTURE0);
    // "Low Poly Light Bulb" (https://skfb.ly/6VnSG) by AleixoAlonso is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
    LightSource pointLightSource("3D/bulb.obj", glm::vec3(-5, 5, 5), glm::vec3(5.f, 5.f, 5.f), &lightShaderProgram);
    
    // Stores the index of the currently selected model.
    int selectedIndex = 0;
    // Stores all the initialized models.
    std::vector<Model*> vecModels;
    vecModels.push_back(&primaryModel);
    vecModels.push_back(&pointLightSource);

    // Sets the selected model to the currently selected model from the vector of models.
    Model* selectedModel = vecModels[selectedIndex];

    // Stores the mouse cursor positions.
    double x_cursor_pos, y_cursor_pos;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // For light source movement options: Press F to switch between orbiting and free movement.
        pointLightSource.allowFreeMovement(allowFreeMovement);

        // For Panning perspective Camera
        if (mainCamera == &persCamera) {
            // Get the mouse position.
            glfwGetCursorPos(window, &x_cursor_pos, &y_cursor_pos);
            // Calculate the mouse position with 0, 0 at the center of the screen.
            glm::vec2 mousePos = glm::vec2(x_cursor_pos - (width / 2), y_cursor_pos - (height / 2));

            // Calculate the position of the perspective camera relative to the mouse position (rotation)
            persCamera.calcMouseToCamera(mousePos);
        }

        // Calculate the view matrix of the currently used camera.
        glm::mat4 viewMatrix = mainCamera->calcViewMatrix();
        
        /* * * * * * * * * * * * * * * * * UPDATE * * * * * * * * * * * * * * * * */
        // Changing to perspective view.
        if (changeToPers) {
            // Set the camera to the perspective camera.
            mainCamera = &persCamera;
            changeToPers = false;
            std::cout << "Currently using Perspective Camera." << std::endl;
        }

        // Chaning to Orthographic view.
        if (changeToOrtho) {
            // Set the camera to the orthographic camera.
            mainCamera = &orthoCamera;
            changeToOrtho = false;
            std::cout << "Currently using Orthographic Camera." << std::endl;
        }

        // Changing the selected model.
        if (changeSelected) {
            // If it's not 0, it's 1 and vice versa.
            if (selectedIndex == 0) selectedIndex = 1;
            else selectedIndex = 0;

            // Get the model at the newly changed index and set it to selected.
            selectedModel = vecModels[selectedIndex];
            changeSelected = false;
            std::cout << "Switched selected object." << std::endl;
        }

        // If the point light is currently selected
        if (selectedIndex == 1) {
            // Change the color to green since it's selected.
            pointLightSource.setColor(green);

            // Allow the changing of the intensity for both the point light and the directional light.
            if (pointIncIntensity) pointLightSource.getLight()->changeIntensity(speed);
            if (pointDecIntensity) pointLightSource.getLight()->changeIntensity(-speed);
            if (dirIncIntensity) dirLight.changeIntensity(-speed);
            if (dirDecIntensity) dirLight.changeIntensity(speed);
        }
        else {
            // Change the color to white if it is not currently selected.
            pointLightSource.setColor(white);
        }

        // Rotate the currently selected model based on what axis the user is inputting.
        if (isRotatingUp) selectedModel->rotate(speed, 0, 0);
        if (isRotatingDown) selectedModel->rotate(-speed, 0, 0);
        if (isRotatingRight) selectedModel->rotate(0, speed, 0);
        if (isRotatingLeft) selectedModel->rotate(0, -speed, 0);
        if (isRotatingCounter) selectedModel->rotate(0, 0, speed);
        if (isRotatingClockwise) selectedModel->rotate(0, 0, -speed);
        
        // Adding the lighting to the shader
        pointLightSource.getLight()->applyToShader(&shaderProgram, mainCamera->getCameraPos());
        dirLight.applyToShader(&shaderProgram, mainCamera->getCameraPos());

        // Drawing the models.
        for (Model* model : vecModels) {
            model->draw(mainCamera->getProjection(), viewMatrix);
        }

        // Sets the shaderprogram back so that the model renders
        glUseProgram(shaderProgram);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */ 
        glfwPollEvents();
    }

    for (Model* model : vecModels) {
        model->deleteBuffers();
    }

    glfwTerminate();
    return 0;
}