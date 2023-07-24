#ifndef LIGHTING_POINT_LIGHT_H
#define LIGHTING_POINT_LIGHT_H

#include "Light.h"

namespace lighting {

	class PointLight : public Light {
		public:
			PointLight(glm::vec3 lightPos);

		public:
			void applyUniqueValuesToShader(GLuint* shaderProgram, glm::vec3 cameraPos);
	};
}

#endif // !LIGHTING_POINT_LIGHT_H
