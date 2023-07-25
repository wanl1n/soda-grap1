#include "OrthoCamera.h"

using namespace cameras;

OrthoCamera::OrthoCamera(float height, float width) : MyCamera(height, width) {
    this->cameraPos = glm::vec3(0, 50, 0);
    this->cameraCenter = glm::vec3(0, 0, 0);
    this->worldUp = glm::vec3(0, 0, 1);
	this->createProjection();
}

void OrthoCamera::createProjection() {
    this->projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, -100.f, 100.f);
}

glm::mat4 OrthoCamera::calcViewMatrix() {
    this->viewMatrix = glm::lookAt(this->cameraPos,
                                    this->cameraCenter,
                                    this->worldUp);

    return this->viewMatrix;
}