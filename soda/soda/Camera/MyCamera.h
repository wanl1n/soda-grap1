#ifndef CAMERAS_MY_CAMERA_H
#define CAMERAS_MY_CAMERA_H	

#include "../soda.h"

namespace cameras {
	class MyCamera {
		protected:
			glm::mat4 projection;
			glm::vec3 cameraCenter;
			glm::vec3 worldUp;
			glm::mat4 viewMatrix;
			glm::vec3 cameraPos;

			float radius;
			float theta_tot;
			float speed;
			float height;
			float width;

		public:
			MyCamera(float height, float width, float radius = 30.f, float speed = 0.01f);

			// These functions will be defined in the child classes.
			virtual void createProjection() = 0;
			virtual glm::mat4 calcViewMatrix() = 0;

		public:
			glm::vec3 getCameraPos();
			glm::mat4 getProjection();
	};
}

#endif