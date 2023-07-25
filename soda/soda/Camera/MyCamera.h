#ifndef CAMERAS_MY_CAMERA_H
#define CAMERAS_MY_CAMERA_H	

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cameras {
	class MyCamera {
		protected:
			glm::mat4 projection;
			glm::vec3 cameraCenter;
			glm::vec3 worldUp;
			glm::mat4 viewMatrix;
			glm::vec3 cameraPos;

			float radius; // Distance of the spawned object to the camera.
			float theta_tot; // Max angle on the left or bottom side of the screen.
			float speed;
			float height;
			float width;

		public:
			MyCamera(float height, float width, float radius = 30.f, float speed = 0.01f);
			virtual void createProjection() = 0;
			virtual glm::mat4 calcViewMatrix() = 0;

		public:
			glm::vec3 getCameraPos();
			glm::mat4 getProjection();
	};
}

#endif