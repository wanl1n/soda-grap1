#ifndef MODELS_LIGHT_SOURCE_H
#define MODELS_LIGHT_SOURCE_H

#include "Model.h"
#include "../Light/PointLight.h"

namespace models {
	using namespace lighting;

	class LightSource : public Model {
		private:
			glm::vec4 modelColor;

			std::vector<GLuint> mesh_indices;
			GLuint* shaderProgram;
			GLuint VAO, VBO, EBO;

			tinyobj::attrib_t attributes;

			PointLight light;

		public:
			LightSource(std::string path, glm::vec3 pos, glm::vec3 scale,
						GLuint* shaderProgram);
			void loadModelData(std::string path);

		public:
			void generateBuffers();
			void draw(glm::mat4 projection, glm::mat4 viewMatrix);
			void deleteBuffers();
			void rotate(float thetaX, float thetaY, float thetaZ);

		public:
			PointLight* getLight();
			void setColor(glm::vec4 color);
	};
}


#endif // !MODELS_LIGHT_SOURCE_H
