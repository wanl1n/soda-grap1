#include "MyCamera.h"

using namespace cameras;

MyCamera::MyCamera(float height, float width, float radius, float speed) : 
    cameraPos(0, 0, -10.f), viewMatrix(1.0f), projection(1.0f),
    cameraCenter(0, 0, 0),
    worldUp(0, 0, 0) {
    // Initialize with placeholder values to be altered when necessary in the child classes.

    this->radius = radius; // Distance of the camera to the object.
    this->theta_tot = 90.f; // Max angle on the left or bottom side of the screen.
    this->speed = speed; // Speed of the camera movement.
    this->height = height; // height of the screen.
    this->width = width; // width of the screen.
}

// Returns the projection of the camera.
glm::mat4 MyCamera::getProjection() {
    return this->projection;
}

// Returns the camera position of the current camera.
glm::vec3 MyCamera::getCameraPos() {
    return this->cameraPos;
}