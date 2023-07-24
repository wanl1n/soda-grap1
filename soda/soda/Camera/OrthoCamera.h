#ifndef CAMERAS_ORTHO_CAMERA_H
#define CAEMRAS_ORTHO_CAMERA_H

#include "MyCamera.h"

namespace cameras {
	class OrthoCamera : public MyCamera {

		public:
			OrthoCamera(float height, float width);

		public:
			void createProjection();
	};
}

#endif