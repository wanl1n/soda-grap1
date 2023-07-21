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

float speed = 0.5f;
float x_mod = 0;
float y_mod = 0;
float z_mod = 0.f;
//float r_mod = 0;
//float g_mod = 0;
//float b_mod = 0;

float scale_mod = 0.05f;

float xrot_mod = 0.f;
float yrot_mod = 0.f;

float fov_mod = 60.f;

bool isMovingUp = false;
bool isMovingDown = false;
bool isMovingLeft = false;
bool isMovingRight = false;
bool isMovingFront = false;
bool isMovingBack = false;

bool isRotatingUp = false;
bool isRotatingDown = false;
bool isRotatingRight = false;
bool isRotatingLeft = false;

bool isScalingUp = false;
bool isScalingDown = false;

bool isZoomingIn = false;
bool isZoomingOut = false;

bool isChangeLight = false;

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
    
    /*if (key == GLFW_KEY_F && action == GLFW_PRESS)
        r_mod -= 1.f;
    if (key == GLFW_KEY_G && action == GLFW_PRESS)
        g_mod -= 1.f;
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
        b_mod -= 1.f;
    if (key == GLFW_KEY_F && action == GLFW_RELEASE)
        r_mod += 1.f;
    if (key == GLFW_KEY_G && action == GLFW_RELEASE)
        g_mod += 1.f;
    if (key == GLFW_KEY_H && action == GLFW_RELEASE)
        b_mod += 1.f;*/

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        isRotatingRight = true;
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        isRotatingRight = false;
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        isRotatingUp = true;
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
        isRotatingUp = false;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        isRotatingDown = true;
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
        isRotatingDown = false;
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        isRotatingLeft = true;
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        isRotatingLeft = false;

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        isScalingDown = true;
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
        isScalingDown = false;
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        isScalingUp = true;
    if (key == GLFW_KEY_E && action == GLFW_RELEASE)
        isScalingUp = false;

    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        isZoomingIn = true;
    if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
        isZoomingIn = false;
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        isZoomingOut = true;
    if (key == GLFW_KEY_X && action == GLFW_RELEASE)
        isZoomingOut = false;

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        isChangeLight = true;
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        isChangeLight = false;
}

void scroll_callback(
    GLFWwindow* window, 
    double xoffset, 
    double yoffset)
{
    isMovingFront = false;
    isMovingBack = false;
    if (yoffset < 0)
        z_mod -= speed;
        //isMovingBack = true;
    if (yoffset > 0)
        z_mod += speed;
        //isMovingFront = true;
}

GLuint LoadImage(const char* path, unsigned int texture_ind) {
    int img_width, img_height, color_channels; // Width, Height, and color channels of the Texture.

    // Fix the flipped texture (by default it is flipped).
    stbi_set_flip_vertically_on_load(true);
    // Load the texture and fill out the variables.
    unsigned char* text_bytes = stbi_load(path, // Texture path
        &img_width, // Width of the texture
        &img_height, // height of the texture
        &color_channels, // color channel
        0);

    // OpenGL reference to the texture.
    GLuint texture;
    // Generate a reference.
    glGenTextures(1, &texture);
    // Set the current texture we're working on to Texture 0.
    glActiveTexture(texture_ind);
    // Bind our next tasks to Tex0 to our current reference similar to VBOs.
    glBindTexture(GL_TEXTURE_2D, texture);
    //If you want to set how the texture maps on a different size model
    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S, // XY = ST (s for x, t for y)
        GL_CLAMP_TO_EDGE //GL_CLAMP_TO_EDGE for stretch, 
    );
    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T, // XY = ST (s for x, t for y)
        GL_REPEAT //GL_CLAMP_TO_EDGE for stretch, 
    );

    unsigned int rgb = GL_RGB;
    if (color_channels == 3) {
        rgb = GL_RGB;
    } else if (color_channels == 4) {
        rgb = GL_RGBA;
    }

    //Assign the loaded texture to the OpenGL reference.
    glTexImage2D(
        GL_TEXTURE_2D,
        0, // Texture 0
        rgb, // Target color format of the texture.
        img_width, // Texture width
        img_height, // Texture height
        0,
        rgb, // Color format of the texture
        GL_UNSIGNED_BYTE,
        text_bytes // loaded texture in bytes
    );

    // Generate the mipmaps to the current texture
    glGenerateMipmap(GL_TEXTURE_2D);
    // Free up the loaded bytes.
    stbi_image_free(text_bytes);

    return texture;
}

int main(void)
{
    GLFWwindow* window;
    // Center of window is 0,0
    // Sides are 1 or -1.

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

    GLuint texture = LoadImage("3D/banancat.png", GL_TEXTURE0);
    GLuint norm_tex = LoadImage("3D/bananorm.png", GL_TEXTURE1);

    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST);

    //glViewport(0, 0, 300, 600); // changes the size of the viewport; used for splitscreen etc.

    // Gets user input.
    glfwSetKeyCallback(window, Key_Callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Vertex shader for positioning
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    // Fragment shader for coloring
    std::fstream fragSrc("Shaders/sample.frag");
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

    // Vertex shader for positioning
    std::fstream sky_vertSrc("Shaders/skybox.vert");
    std::stringstream sky_vertBuff;
    sky_vertBuff << sky_vertSrc.rdbuf();
    std::string sky_vertS = sky_vertBuff.str();
    const char* sky_v = sky_vertS.c_str();

    // Fragment shader for coloring
    std::fstream sky_fragSrc("Shaders/skybox.frag");
    std::stringstream sky_fragBuff;
    sky_fragBuff << sky_fragSrc.rdbuf();
    std::string sky_fragS = sky_fragBuff.str();
    const char* sky_f = sky_fragS.c_str();

    // Creating the vertex shader for use in the program.
    GLuint sky_vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sky_vertShader, 1, &sky_v, NULL);
    glCompileShader(sky_vertShader);

    // Creating the fragment shader for use in the program.
    GLuint sky_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sky_fragShader, 1, &sky_f, NULL);
    glCompileShader(sky_fragShader);

    // Creating the shader program.
    GLuint skyboxProgram = glCreateProgram();
    glAttachShader(skyboxProgram, sky_vertShader);
    glAttachShader(skyboxProgram, sky_fragShader);

    // Links the shader to the program.
    glLinkProgram(skyboxProgram);

    glDeleteShader(sky_vertShader);
    glDeleteShader(sky_fragShader);

    // Bunny Object elements
    std::string path = "3D/banancat.obj";
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

    /*
      7--------6
     /|       /|
    4--------5 |
    | |      | |
    | 3------|-2
    |/       |/
    0--------1
    */
    //Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    // Order is particular (right, left, up, down, front, back)
    std::string facesSkybox[]{
        "Skybox/rainbow_rt.png", // Start with right face
        "Skybox/rainbow_lf.png",
        "Skybox/rainbow_up.png",
        "Skybox/rainbow_dn.png",
        "Skybox/rainbow_ft.png",
        "Skybox/rainbow_bk.png"
    };

    unsigned int skyboxTex;
    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    // Avoid pixelation
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Reduce pixelation as much if the texture is too big
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Reduce pixelation as much if the texture is too small

    // 3d cube on all axes clamp.
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // To prevent tiling
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // To prevent tiling
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // To prevent tiling

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false); // dont flip the skybox.

        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);
        
        if (data) {
            // this png has no alpha channel
            // cubemap face address starts at the right (positive x)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }

        stbi_image_free(data);
    }

    stbi_set_flip_vertically_on_load(true);

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shape[0].mesh.indices[i].vertex_index
        );
    }

    //GLfloat UV[]{
    //    0.f, 2.f,
    //    0.f, 0.f,
    //    2.f, 2.f,
    //    2.f, 0.f,
    //    2.f, 2.f,
    //    2.f, 0.f,
    //    0.f, 2.f,
    //    0.f, 0.f
    //};

    //GLfloat vertices[]{
    //  // x,    y,    z
    //    0.0f, 0.5f, 0.0f, // Vertex 0
    //    -0.5f, -0.5f, 0.f,// Vertex 1
    //    0.5f, -0.5f, 0.f  // Vertex 2
    //};

    //GLuint indices[]{
    //    0, 1, 2
    //};

    //Vector to hold our tangents
    std::vector<glm::vec3> tangents;
    //Vector to hold our bitangents
    std::vector<glm::vec3> bitangents;

    for(int i = 0; i < shape[0].mesh.indices.size(); i += 3) {

        // Get the 3 vertex data for the triangle
        tinyobj::index_t vData1 = shape[0].mesh.indices[i];
        tinyobj::index_t vData2 = shape[0].mesh.indices[i+1];
        tinyobj::index_t vData3 = shape[0].mesh.indices[i+2];

        // Position of vertex 1
        glm::vec3 v1 = glm::vec3(attributes.vertices[vData1.vertex_index * 3],
                                attributes.vertices[vData1.vertex_index * 3] + 1,
                                attributes.vertices[vData1.vertex_index * 3] + 2);
        // Position of vertex 2
        glm::vec3 v2 = glm::vec3(attributes.vertices[vData2.vertex_index * 3],
                                attributes.vertices[vData2.vertex_index * 3] + 1,
                                attributes.vertices[vData2.vertex_index * 3] + 2);
        //position of vertex 3
        glm::vec3 v3 = glm::vec3(attributes.vertices[vData3.vertex_index * 3],
                                attributes.vertices[vData3.vertex_index * 3] + 1,
                                attributes.vertices[vData3.vertex_index * 3] + 2);

        // Position of uv 1
        glm::vec2 uv1 = glm::vec2(attributes.texcoords[vData1.texcoord_index * 2],
                                attributes.texcoords[vData1.texcoord_index * 2] + 1);
        // Position of uv 2
        glm::vec2 uv2 = glm::vec2(attributes.texcoords[vData2.texcoord_index * 2],
                                attributes.texcoords[vData2.texcoord_index * 2] + 1);
        //position of uv 3
        glm::vec2 uv3 = glm::vec2(attributes.texcoords[vData3.texcoord_index * 2],
                                attributes.texcoords[vData3.texcoord_index * 2] + 1);

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));

        // Tangent (T)
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        // Bitangent (B)
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }
    
    // Loading the UV data of the object.
    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shape[0].mesh.indices[i];

        // This will generate an array with 3 consecutive points 
        // as a position coordinate ordered in the vector.
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);// X
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);// Y
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);// Z

        // Add normals here
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3]);// X
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 1]);// Y
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 2]);// Z

        // UV
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);

        fullVertexData.push_back(tangents[i].x);
        fullVertexData.push_back(tangents[i].y);
        fullVertexData.push_back(tangents[i].z);

        fullVertexData.push_back(bitangents[i].x);
        fullVertexData.push_back(bitangents[i].y);
        fullVertexData.push_back(bitangents[i].z);
        // Result: 3 Pos, 3 Norm, 2 UV, 3 Pos, 3 Norm, 2 UV, and so on.
    }


    GLuint VAO, VBO;
        //EBO, VBO_UV;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &VBO_UV); // Generate UV Buffer
    //glGenBuffers(1, &EBO);

    // We're working with this VAO.
    glBindVertexArray(VAO);
    // We're working with this VBO.
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Binds VBO to VAO.
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        /*sizeof(GLfloat) * attributes.vertices.size(),
        &attributes.vertices[0],*/
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
        //XYZ normals UV (change from 3 to 5) add TB
        (5 + 3 + 6) * sizeof(GL_FLOAT), // Size of the vertex data
        (void*)0
    );

    GLintptr normPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GLfloat),
        (void*)normPtr
    );

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GLfloat),
        (void*)uvPtr
    );

    GLintptr tangentPtr = 8 * sizeof(float);
    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GLfloat),
        (void*)tangentPtr
    );

    GLintptr bitangentPtr = 11 * sizeof(float);
    glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GLfloat),
        (void*)bitangentPtr
    );


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1); // for Normals
    glEnableVertexAttribArray(2); // 2 for UV / Texture
    glEnableVertexAttribArray(3); // 3 for Tangent
    glEnableVertexAttribArray(4); // 4 for Bitangent

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(
    //    GL_ELEMENT_ARRAY_BUFFER,
    //    sizeof(GLuint) * mesh_indices.size(),
    //    mesh_indices.data(),
    //    //sizeof(indices), 
    //    //indices,
    //    GL_STATIC_DRAW
    //);

    //// Bind the UV Buffer
    //glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
    //// Add in the buffer data
    //glBufferData(GL_ARRAY_BUFFER,
    //             sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])), //float * size of the UV array
    //             &UV[0], // The UV array earlier
    //             GL_DYNAMIC_DRAW
    //);
    //// How to interpret array above:
    //glVertexAttribPointer(
    //    2, // Index 2 for UV
    //    2, // UV
    //    GL_FLOAT,
    //    GL_FALSE,
    //    2 * sizeof(float),
    //    (void*)0
    //);

    // Clean Up
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Wala nang ginagalaw sa VBO.

    glBindVertexArray(0); // Wala ka nang ginagalaw na VAO.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // EBO

    // Creating identity matrices
    /*glm::mat3 identity_matrix3 = glm::mat3(1.0f);
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);*/

    // Creating an orthographic view.
    //glm::mat4 projection = glm::ortho(-2.f, 2.f, -2.f, 2.f, -1.f, 1.f); // left most point, right most point, bottom most point, top most point, z near, z far

    // Creating a perspective view.
    glm::mat4 projection = glm::perspective(
        glm::radians(60.f), // FOV
        height / width,
        0.1f,
        100.f
    );

    //// Create a 3D translation matrix
    //glm::mat4 translation = glm::translate(identity_matrix4, 
    //                                       glm::vec3(x, y, z));
    //glm::mat4 scale = glm::scale(identity_matrix4,
    //                             glm::vec3(x, y, z));
    //glm::mat4 rotation = glm::rotate(identity_matrix4,
    //                                 glm::radians(theta),
    //                                 glm::vec3(x, y, z));

    // Position of light
    glm::vec3 lightPos = glm::vec3(0, 0, 10);
    // Light Color
    glm::vec3 lightColor = glm::vec3(1, 1.f, 1.f);
    // Ambient light strength
    float ambientStr = 0.1f;
    glm::vec3 ambientColor = lightColor;
    //Specular Light
    float specStr = 10.f;
    float specPhong = 16;

    float intensityMultiplier = 10;

    // Stores the mouse cursor positions.
    double x_cursor_pos, y_cursor_pos;

    // Initialize the camera position.
    glm::vec3 cameraPos = glm::vec3(x_mod, y_mod, z_mod);

    // Enable Blending
    glEnable(GL_BLEND);
    //Choose a Blending Function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the depth buffer as well.
        
        /* * * * * * * * * * * * SETTING UP THE VIEW MATRIX * * * * * * * * * * * */
        // Making the camera variables and setting up.
        //glm::vec3 cameraPos = glm::vec3(0, 0, 10.f);
        /*glm::mat4 cameraPosMatrix = glm::translate(glm::mat4(1.0f), cameraPos * -1.f);*/
        //glm::vec3 worldUp = glm::normalize(glm::vec3(0, 1.f, 0));
        //glm::vec3 cameraCenter = glm::vec3(0, 0, 0);

        // Making the vectors of the camera. ---- if not using lookAt().
        //glm::vec3 F = (cameraCenter - cameraPos); // Forward Vector
        //F = glm::normalize(F);
        //glm::vec3 R = glm::cross(F, worldUp); // Right Vector, No need to normalize because F is normalized.
        //glm::vec3 U = glm::cross(R, F); // Up Vector

        // Making the camera orientation matrix. ---- if not using lookAt().
        /*glm::mat4 cameraOrientation = glm::mat4(1.0f);*/

        //// matrix[col][row]
        //cameraOrientation[0][0] = R.x;
        //cameraOrientation[1][0] = R.y;
        //cameraOrientation[2][0] = R.z;
        //cameraOrientation[0][1] = U.x;
        //cameraOrientation[1][1] = U.y;
        //cameraOrientation[2][1] = U.z;
        //cameraOrientation[0][2] = -F.x;
        //cameraOrientation[1][2] = -F.y;
        //cameraOrientation[2][2] = -F.z;

        // Final Computation for the View Matrix.
        //glm::mat4 viewMatrix = cameraOrientation * cameraPosMatrix;
        //glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraCenter, worldUp); // For this, no need to make the vectors.
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        /* * * * * ADJUSTING THE CAMERA ACCORDING TO THE MOUSE POSITION * * * * */
        // Get the position of the cursor in the window.
        glfwGetCursorPos(window, &x_cursor_pos, &y_cursor_pos);

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
            cameraPos + cameraCenter, // to make sure cameracenter is always infront of camera pos.
            worldUp);

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        glUseProgram(skyboxProgram);
        
        glm::mat4 skyView = glm::mat4(1.f);
        // Strip off the translation of the camera since we only need the rotation.
        skyView = glm::mat4(glm::mat3(viewMatrix));

        unsigned int skyProjectionLoc = glGetUniformLocation(skyboxProgram, "projection");
        glUniformMatrix4fv(skyProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int skyViewLoc = glGetUniformLocation(skyboxProgram, "view");
        glUniformMatrix4fv(skyViewLoc, 1, GL_FALSE, glm::value_ptr(skyView));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glUseProgram(shaderProgram);

        /* * * * * * * * * * * * * * * * * UPDATE * * * * * * * * * * * * * * * * */
        if (isMovingUp) intensityMultiplier += speed;
        if (isMovingDown) intensityMultiplier -= speed;
        if (isMovingLeft) x_mod -= speed;
        if (isMovingRight) x_mod += speed;

        if (isScalingUp) scale_mod += speed;
        if (isScalingDown && scale_mod >= 0.05f) scale_mod -= speed;

        if (isZoomingIn) fov_mod -= speed;
        if (isZoomingOut) fov_mod += speed;

        if (isRotatingUp) xrot_mod += speed;
        if (isRotatingDown) xrot_mod -= speed;
        if (isRotatingRight) yrot_mod += speed;
        if (isRotatingLeft) yrot_mod -= speed;

        lightPos = glm::vec3(x_mod, 0, 10);
        /*if (isChangeLight) lightColor = glm::vec3(1, 1, 1);
        if (!isChangeLight) lightColor = glm::vec3(1, 0.75f, 0.8f);*/
        /*if (isChangeLight) intensityMultiplier = 0;
        if (!isChangeLight) intensityMultiplier = 1000;*/

        /*unsigned int rCol = glGetUniformLocation(shaderProgram, "r");
        glUniform1f(rCol, r_mod);
        unsigned int gCol = glGetUniformLocation(shaderProgram, "g");
        glUniform1f(gCol, g_mod);
        unsigned int bCol = glGetUniformLocation(shaderProgram, "b");
        glUniform1f(bCol, b_mod);

        projection = glm::perspective(
            glm::radians(fov_mod), // FOV
            height / width,
            0.1f,
            100.f
        ); */
        
        /* * * * * * * * * * * * APPLYING THE TEXTURE * * * * * * * * * * * * * */
        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        glActiveTexture(GL_TEXTURE1);
        GLuint tex1Address = glGetUniformLocation(shaderProgram, "norm_tex");
        glBindTexture(GL_TEXTURE_2D, norm_tex);
        glUniform1i(tex1Address, 1);
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        /* * * * * * CREATING THE TRANSFORMATION MATRIX FOR THE MODEL  * * * * * */
        glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0f),
            glm::vec3(0, -1.f, z_mod));

        transformation_matrix = glm::scale(transformation_matrix,
            glm::vec3(scale_mod, scale_mod, scale_mod));

        transformation_matrix = glm::rotate(transformation_matrix,
            glm::radians(xrot_mod),
            glm::vec3(1.f, 0.f, 0.f));

        transformation_matrix = glm::rotate(transformation_matrix,
            glm::radians(yrot_mod),
            glm::vec3(0.f, 1.f, 0.f));

        

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        /* * * * * * * * * * * APPLYING TO VERTEX SHADER * * * * * * * * * * * * */
        // Gets the transformed position from the Shader program.
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(
            transformLoc,
            1,
            GL_FALSE,
            glm::value_ptr(transformation_matrix)
        );

        // Gets the view matrix from the shader program.
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(
            viewLoc,
            1,
            GL_FALSE,
            glm::value_ptr(viewMatrix)
        );

        // Gets the projected position from the shader program.
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(
            projLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        // LIGHTING
        GLuint lightAddress = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));
        GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));
        
        GLuint ambientStrAddress = glGetUniformLocation(shaderProgram, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);
        GLuint ambientColorAddress = glGetUniformLocation(shaderProgram, "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

        GLuint cameraPosAddress = glGetUniformLocation(shaderProgram, "cameraPos");
        glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));
        GLuint specStrAddress = glGetUniformLocation(shaderProgram, "specStr");
        glUniform1f(specStrAddress, specStr);
        GLuint specPhongAddress = glGetUniformLocation(shaderProgram, "specPhong");
        glUniform1f(specPhongAddress, specPhong);

        float distance = sqrt(lightPos.x * lightPos.x + lightPos.y * lightPos.y + lightPos.z * lightPos.z);
        GLuint distanceAddress = glGetUniformLocation(shaderProgram, "dist");
        glUniform1f(distanceAddress, distance);
        GLuint intensityMultiplierAddress = glGetUniformLocation(shaderProgram, "intensityMult");
        glUniform1f(intensityMultiplierAddress, intensityMultiplier);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shaderProgram);

        //glDrawElements(
        //    GL_TRIANGLES,
        //    //sizeof(indices),
        //    mesh_indices.size(),
        //    GL_UNSIGNED_INT,
        //    0
        //);

        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / (5+3+6));

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
    //glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}