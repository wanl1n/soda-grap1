#ifndef CAMERAS_PERS_CAMERA_H
#define CAEMRAS_PERS_CAMERA_H

#include "MyCamera.h"

namespace cameras {
	class PerspectiveCamera : public MyCamera {

		public:
			PerspectiveCamera(float height, float width);

		public:
			void createProjection();
	};
}

#endif