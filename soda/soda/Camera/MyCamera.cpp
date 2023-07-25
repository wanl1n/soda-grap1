#include "MyCamera.h"

using namespace cameras;

MyCamera::MyCamera(float height, float width, float radius, float speed) : 
    cameraPos(0, 0, -10.f), viewMatrix(1.0f), projection(1.0f),
    cameraCenter(0, 0, 0),
    worldUp(0, 0, 0) {

    this->radius = radius; // Distance of the spawned object to the camera.
    this->theta_tot = -90.f; // Max angle on the left or bottom side of the screen.
    this->speed = speed;
    this->height = height;
    this->width = width;
}

glm::mat4 MyCamera::getProjection() {
    return this->projection;
}

glm::vec3 MyCamera::getCameraPos() {
    return this->cameraPos;
}