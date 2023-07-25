#include "PerspectiveCamera.h"

using namespace cameras;

PerspectiveCamera::PerspectiveCamera(float height, float width) : 
                                    MyCamera(height, width) {
    this->createProjection();
}

void PerspectiveCamera::createProjection() {
    this->projection = glm::perspective(glm::radians(60.f),
        height / width,
        0.1f,
        100.f);
}

void PerspectiveCamera::calcMouseToCamera(glm::vec2 mousePos) {
    float yaw = glm::radians((mousePos.x / (width / 2)) * this->theta_tot);
    float pitch = glm::radians((mousePos.y / (height / 2)) * this->theta_tot);

    // Limiting the degree in case of flipping.
    if (yaw > 89.9f) yaw = 89.9f;
    if (yaw < -89.9f) yaw = -89.9f;
    if (pitch > 89.9f) pitch = 89.9f;
    if (pitch < -89.9f) pitch = -89.9f;

    // Finally get the direction in each axis by using Polar to Cartesian point conversion.
    float xAxisRot = this->radius * sin(yaw) * cos(pitch);
    float yAxisRot = this->radius * sin(pitch);
    float zAxisRot = this->radius * cos(yaw) * cos(pitch);

    // Update the camera center with the new calculated point.
    // Finally, make sure to add the strafing movement of the camera to the x-axis.
    this->cameraCenter = glm::vec3(xAxisRot, yAxisRot, zAxisRot);

    // Next, calculate the position change based on where the camera center is.
    this->worldUp = glm::normalize(glm::vec3(0, 1.f, 0));
}

glm::mat4 PerspectiveCamera::calcViewMatrix() {
    // Create the view matrix.
    this->viewMatrix = glm::lookAt(this->cameraPos,
        this->cameraPos + this->cameraCenter,
        this->worldUp);

    return this->viewMatrix;
}