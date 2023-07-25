#include "OrthoCamera.h"

using namespace cameras;

OrthoCamera::OrthoCamera(float height, float width) : MyCamera(height, width) {
    // To make the orthographic camera top view,
    // First, set the position to a point above the objects
    // Second, set the camera center to the center/location of the main model
    // Finally, set the world up to the z-axis so that it's looking down.
    this->cameraPos = glm::vec3(0, 50, 0);
    this->cameraCenter = glm::vec3(0, 0, 0);
    this->worldUp = glm::vec3(0, 0, 1);

    // Create the projection of the camera for view.
	this->createProjection();
}

// Creates the orthographic projection of the world.
void OrthoCamera::createProjection() {
    // Set the bounds to -10 to 10 for x-axis and y-axis. 
    // Set the bounds to -100 to 100 for the z-axis.
    this->projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, -100.f, 100.f);
}

// Calculates the view matrix using the position, center and the world up.
glm::mat4 OrthoCamera::calcViewMatrix() {
    this->viewMatrix = glm::lookAt(this->cameraPos,
                                    this->cameraCenter,
                                    this->worldUp);

    return this->viewMatrix;
}