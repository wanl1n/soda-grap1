#ifndef MODELS_TEXTURED_MODEL_H
#define MODELS_TEXTURED_MODEL_H

#include "Model.h"

namespace models {

	class TexturedModel : public Model {
		private:
			GLuint texture;
			GLuint textureIndex;

			std::vector<GLfloat> fullVertexData;
			GLuint* shaderProgram;
			GLuint VAO, VBO;

		public:
			TexturedModel(std::string path, glm::vec3 pos, glm::vec3 scale,
						GLuint* shaderProgram,
						const char* texturePath,
						GLuint texture_ind);
			void loadModelData(std::string path);
			void generateBuffers();
			void loadTexture(const char* path, GLuint texture_ind);

		public:
			void draw(glm::mat4 projection, glm::mat4 viewMatrix);
			void deleteBuffers();
	};
}


#endif // !MODELS_TEXTURED_MODEL_H
