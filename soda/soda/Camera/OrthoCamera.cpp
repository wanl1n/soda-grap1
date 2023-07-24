#include "OrthoCamera.h"

using namespace cameras;

OrthoCamera::OrthoCamera(float height, float width) : MyCamera(height, width) {
	this->createProjection();
}

void OrthoCamera::createProjection() {
	this->projection = glm::ortho(-2.f, 2.f, -2.f, 2.f, -50.f, 100.f);
}