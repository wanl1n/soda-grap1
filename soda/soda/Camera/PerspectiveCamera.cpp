#include "PerspectiveCamera.h"

using namespace cameras;

PerspectiveCamera::PerspectiveCamera(float height, float width) : 
                                    MyCamera(height, width) {
    // Create the projection matrix first.
    this->createProjection();

    // Set the radius to an amount enough to see both objects.
    this->radius = 20.f;
}

// Creates the perspective projection of the world based on the height and width and the FOV set.
void PerspectiveCamera::createProjection() {
    this->projection = glm::perspective(glm::radians(60.f),
        height / width,
        0.1f,
        100.f);
}

// Calculate the rotation of the camera around the object 
// based on the position of the mouse on the screen.
void PerspectiveCamera::calcMouseToCamera(glm::vec2 mousePos) {

    // Calculate the rotation axes calculated using the mouse position and the angle limit of the screen.
    float yaw = glm::radians((mousePos.x / (width / 2)) * this->theta_tot);
    float pitch = glm::radians((mousePos.y / (height / 2)) * this->theta_tot);

    // Limiting the degree in case of flipping.
    float limit = this->theta_tot - 0.1f;
    if (yaw > limit) yaw = limit;
    if (yaw < -limit) yaw = -limit;
    if (pitch > limit) pitch = limit;
    if (pitch < -limit) pitch = -limit;

    // Finally get the direction in each axis by using Polar to Cartesian point conversion.
    float xAxisRot = this->radius * sin(yaw) * cos(pitch);
    float yAxisRot = this->radius * sin(pitch);
    float zAxisRot = this->radius * cos(yaw) * cos(pitch);

    // Update the camera position with the new calculated point.
    this->cameraPos = glm::vec3(xAxisRot, yAxisRot, zAxisRot);

    // Next, set the world Up vector to the y-axis.
    this->worldUp = glm::normalize(glm::vec3(0, 1.f, 0));
}

// Calculates the view matrix using the position, center and the world up.
glm::mat4 PerspectiveCamera::calcViewMatrix() {
    this->viewMatrix = glm::lookAt(this->cameraPos,
        this->cameraCenter,
        this->worldUp);

    return this->viewMatrix;
}