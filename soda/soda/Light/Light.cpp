#include "Light.h"

using namespace lighting;

Light::Light(glm::vec3 lightPos) {

    // Assigns the passed light position as the position of this light.
    this->lightPos = lightPos;
    // Sets the light color to white.
    this->lightColor = glm::vec3(1, 1.f, 1.f);

    // Sets the ambient strength to 0.1f so it's not too small.
    this->ambientStr = 0.1f;
    // Sets the ambient color to the light color.
    this->ambientColor = lightColor;

    // Sets the specular light strength to 10.f and specular phong to 16.
    this->specStr = 10.f;
    this->specPhong = 16;

    // Sets the intensity multiplier to 10.
    this->intensityMultiplier = 10;

    // Placeholder value for the light type (will be changed by the child class)
    this->lightType = 2;
}

// Passes the common properties to the shader program.
void Light::applyToShader(GLuint* shaderProgram, glm::vec3 cameraPos) {
    
    // Apply the unique values (order doesn't matter)
    this->applyUniqueValuesToShader(shaderProgram, cameraPos);

    // Passes the ambient strength to the shader program.
    GLuint ambientStrAddress = glGetUniformLocation(*shaderProgram, "ambientStr");
    glUniform1f(ambientStrAddress, ambientStr);
    
    // Passes the specular light strength to the shader program.
    GLuint specStrAddress = glGetUniformLocation(*shaderProgram, "specStr");
    glUniform1f(specStrAddress, specStr);
    // Passes the specular phong to the shader program.
    GLuint specPhongAddress = glGetUniformLocation(*shaderProgram, "specPhong");
    glUniform1f(specPhongAddress, specPhong);
    // Passes the light type to the shader program.
    GLuint lightTypeLoc = glGetUniformLocation(*shaderProgram, "lightType");
    glUniform1i(lightTypeLoc, this->lightType);
}

// Accepts offset values and adds them to the light position.
void Light::moveLight(float x_mod, float y_mod, float z_mod) {
    this->lightPos += glm::vec3(x_mod, y_mod, z_mod);
}

// Adds the accepted value to the intensity multiplier to alter.
void Light::changeIntensity(float delta) {
    this->intensityMultiplier += delta;
}

// Sets the color of the light and the ambient color to the accepted color.
void Light::setColor(glm::vec4 color) {
    this->lightColor = color;
    this->ambientColor = color;
}

// Sets the position of the light directly
void Light::setPosition(glm::vec3 pos) {
    this->lightPos = pos;
}
