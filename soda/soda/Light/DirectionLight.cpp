#include "DirectionLight.h"

using namespace lighting;

DirectionLight::DirectionLight(glm::vec3 lightPos) : Light(lightPos) {
	this->lightType = 2;
}