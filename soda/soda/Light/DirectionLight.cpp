#include "DirectionLight.h"

using namespace lighting;

DirectionLight::DirectionLight(glm::vec3 lightPos) : Light(lightPos) {
	// Set the light type 2 for direction light.
    this->lightType = 2;

    // Set the intensity of the direction to 1 so it's not too bright.
    this->intensityMultiplier = 1;
}

// Pass the light properties to the shader. 
// (Has unique values so it doesn't overlap with the other light sources)
void DirectionLight::applyUniqueValuesToShader(GLuint* shaderProgram, glm::vec3 cameraPos) {
    // Passes the light position to the direction light position in the shader
    GLuint lightAddress = glGetUniformLocation(*shaderProgram, "lightPos_d");
    glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));
    // Passes the light color to the direction light color in the shader program.
    GLuint lightColorAddress = glGetUniformLocation(*shaderProgram, "lightColor_d");
    glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));
    // Passes the ambient color to the direction ambient color in the shader program.
    GLuint ambientColorAddress = glGetUniformLocation(*shaderProgram, "ambientColor_d");
    glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));
    // Passes the camera position to the direction camera position in the shader prorgam.
    GLuint cameraPosAddress = glGetUniformLocation(*shaderProgram, "cameraPos_d");
    glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));

    // Calculates the distance of the light position from the center and passes it to the shader program.
    float distance = sqrt(lightPos.x * lightPos.x + lightPos.y * lightPos.y + lightPos.z * lightPos.z);
    GLuint distanceAddress = glGetUniformLocation(*shaderProgram, "dist_d");
    glUniform1f(distanceAddress, distance);
    // Passes the intensity multiplier to the direction light intensity multiplier in the shader program.
    GLuint intensityMultiplierAddress = glGetUniformLocation(*shaderProgram, "intensityMult_d");
    glUniform1f(intensityMultiplierAddress, intensityMultiplier);
}