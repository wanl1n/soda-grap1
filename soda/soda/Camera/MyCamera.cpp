#include "MyCamera.h"

using namespace cameras;

MyCamera::MyCamera(float height, float width, float radius, float speed) : 
    cameraPos(0, 0, -10.f), projection(1.0f), viewMatrix(1.0f), cameraCenter(0, 0, 0), 
    worldUp(0, 0, 0) {

    this->radius = radius; // Distance of the spawned object to the camera.
    this->theta_tot = -90.f; // Max angle on the left or bottom side of the screen.
    this->speed = speed;
    this->height = height;
    this->width = width;
}

void MyCamera::calcMouseToCamera(glm::vec2 mousePos) {
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

glm::mat4 MyCamera::calcViewMatrix() {
    // Create the view matrix.
    this->viewMatrix = glm::lookAt(this->cameraPos,
                                this->cameraPos + this->cameraCenter, 
                                this->worldUp);

    return this->viewMatrix;
}

void MyCamera::moveLeft() {
    this->cameraPos -= glm::normalize(glm::cross(this->cameraCenter, this->worldUp)) * 0.5f;
}

void MyCamera::moveRight() {
    this->cameraPos += glm::normalize(glm::cross(this->cameraCenter, this->worldUp)) * 0.5f;
}

void MyCamera::moveForward() {
    this->cameraPos += this->speed * (this->cameraCenter);
}

void MyCamera::moveBack() {
    this->cameraPos -= this->speed * (this->cameraCenter);
}

glm::mat4 MyCamera::getProjection() {
    return this->projection;
}

glm::vec3 MyCamera::getCameraPos() {
    return this->cameraPos;
}

glm::vec3 MyCamera::getCameraCenter() {
    return this->cameraCenter;
}