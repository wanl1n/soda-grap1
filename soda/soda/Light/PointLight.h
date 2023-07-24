#ifndef LIGHTING_POINT_LIGHT_H
#define LIGHTING_POINT_LIGHT_H

#include "Light.h"

namespace lighting {

	class PointLight : public Light {
		public:
			PointLight(glm::vec3 lightPos);

	};
}

#endif // !LIGHTING_POINT_LIGHT_H
