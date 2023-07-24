#ifndef LIGHTING_LIGHT_H
#define LIGHTING_LIGHT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace lighting {

	class Light {

		protected:
			glm::vec3 lightPos;
			glm::vec3 lightColor;

			float ambientStr;
			glm::vec3 ambientColor;

			float specStr;
			float specPhong;

			float intensityMultiplier;
			int lightType;

		public:
			Light(glm::vec3 lightPos);

		public:
			void applyToShader(GLuint* shaderProgram, glm::vec3 cameraPos);
			virtual void applyUniqueValuesToShader(GLuint* shaderProgram, glm::vec3 cameraPos) = 0;
			void moveLight(float x_mod, float y_mod, float z_mod);
			void changeIntensity(float delta);

		public:
			void setColor(glm::vec4 color);
	};
}

#endif