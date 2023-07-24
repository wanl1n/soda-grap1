#include "PerspectiveCamera.h"

using namespace cameras;

PerspectiveCamera::PerspectiveCamera(float height, float width) : MyCamera(height, width) {
    this->createProjection();
}

void PerspectiveCamera::createProjection() {
    this->projection = glm::perspective(glm::radians(60.f),
        height / width,
        0.1f,
        100.f);
}