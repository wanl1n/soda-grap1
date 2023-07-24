#ifndef LIGHTING_DIRECTION_LIGHT_H
#define LIGHTING_DIRECTION_LIGHT_H

#include "Light.h"

namespace lighting {
	
	class DirectionLight : public Light {
		public:
			DirectionLight(glm::vec3 lightPos);

		public:
			void applyUniqueValuesToShader(GLuint* shaderProgram, glm::vec3 cameraPos);
	};
}

#endif