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
    
    GLuint lightAddress = glGetUniformLocation(*shaderProgram, "lightPos");
    glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));
    GLuint lightColorAddress = glGetUniformLocation(*shaderProgram, "lightColor");
    glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

    GLuint ambientStrAddress = glGetUniformLocation(*shaderProgram, "ambientStr");
    glUniform1f(ambientStrAddress, ambientStr);
    GLuint ambientColorAddress = glGetUniformLocation(*shaderProgram, "ambientColor");
    glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

    GLuint cameraPosAddress = glGetUniformLocation(*shaderProgram, "cameraPos");
    glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));
    GLuint specStrAddress = glGetUniformLocation(*shaderProgram, "specStr");
    glUniform1f(specStrAddress, specStr);
    GLuint specPhongAddress = glGetUniformLocation(*shaderProgram, "specPhong");
    glUniform1f(specPhongAddress, specPhong);

    float distance = sqrt(lightPos.x * lightPos.x + lightPos.y * lightPos.y + lightPos.z * lightPos.z);
    GLuint distanceAddress = glGetUniformLocation(*shaderProgram, "dist");
    glUniform1f(distanceAddress, distance);
    GLuint intensityMultiplierAddress = glGetUniformLocation(*shaderProgram, "intensityMult");
    glUniform1f(intensityMultiplierAddress, intensityMultiplier);

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
