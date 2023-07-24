#include "PointLight.h"

using namespace lighting;

PointLight::PointLight(glm::vec3 lightPos) : Light(lightPos) {
	this->lightType = 1;
}