#include "Light.h"

using namespace lighting;

Light::Light(glm::vec3 lightPos) {

    this->lightPos = lightPos;
    this->lightColor = glm::vec3(1, 1.f, 1.f);

    this->ambientStr = 0.1f;
    this->ambientColor = lightColor;

    this->specStr = 10.f;
    this->specPhong = 16;

    this->intensityMultiplier = 10;
    this->lightType = 2;
}

void Light::applyToShader(GLuint* shaderProgram, glm::vec3 cameraPos) {
    
    this->applyUniqueValuesToShader(shaderProgram, cameraPos);

    GLuint ambientStrAddress = glGetUniformLocation(*shaderProgram, "ambientStr");
    glUniform1f(ambientStrAddress, ambientStr);
    
    GLuint specStrAddress = glGetUniformLocation(*shaderProgram, "specStr");
    glUniform1f(specStrAddress, specStr);
    GLuint specPhongAddress = glGetUniformLocation(*shaderProgram, "specPhong");
    glUniform1f(specPhongAddress, specPhong);

    GLuint lightTypeLoc = glGetUniformLocation(*shaderProgram, "lightType");
    glUniform1i(lightTypeLoc, this->lightType);
}

void Light::moveLight(float x_mod, float y_mod, float z_mod) {
    this->lightPos += glm::vec3(x_mod, y_mod, z_mod);
}

void Light::changeIntensity(float delta) {
    this->intensityMultiplier += delta;
}

void Light::setColor(glm::vec4 color) {
    this->lightColor = color;
}

void Light::setPosition(glm::vec3 pos) {
    this->lightPos = pos;
}
