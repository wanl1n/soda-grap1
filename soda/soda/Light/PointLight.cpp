#include "PointLight.h"

using namespace lighting;

PointLight::PointLight(glm::vec3 lightPos) : Light(lightPos) {
	this->lightType = 1;
}

void PointLight::applyUniqueValuesToShader(GLuint* shaderProgram, glm::vec3 cameraPos) {
    GLuint lightAddress = glGetUniformLocation(*shaderProgram, "lightPos_p");
    glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));
    GLuint lightColorAddress = glGetUniformLocation(*shaderProgram, "lightColor_p");
    glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

    GLuint ambientColorAddress = glGetUniformLocation(*shaderProgram, "ambientColor_p");
    glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

    GLuint cameraPosAddress = glGetUniformLocation(*shaderProgram, "cameraPos_p");
    glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));

    float distance = sqrt(lightPos.x * lightPos.x + lightPos.y * lightPos.y + lightPos.z * lightPos.z);
    GLuint distanceAddress = glGetUniformLocation(*shaderProgram, "dist_p");
    glUniform1f(distanceAddress, distance);

    GLuint intensityMultiplierAddress = glGetUniformLocation(*shaderProgram, "intensityMult_p");
    glUniform1f(intensityMultiplierAddress, intensityMultiplier);
}
