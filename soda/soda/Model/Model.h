#ifndef MODELS_MODEL_H
#define MODELS_MODEL_H

#include "../soda.h"

namespace models {

    class Model {

        protected:
            glm::vec3 pos; // position of spawned model.

            glm::vec3 yAxis; // axis of rotation
            glm::vec3 xAxis; // axis of rotation
            glm::vec3 zAxis; // axis of rotation
            float thetaX; // in degrees
            float thetaY; // in degrees
            float thetaZ; // in degrees

            glm::vec3 scale; // scale of the spawned model.

        public:
            Model(glm::vec3 pos, glm::vec3 scale);
            virtual void loadModelData(std::string) = 0;
        
        public:
            virtual void generateBuffers() = 0;
            virtual void draw(glm::mat4 projection, glm::mat4 viewMatrix) = 0;
            virtual void deleteBuffers() = 0;

        public:
            virtual void rotate(float thetaX, float thetaY, float thetaZ);
    };
}

#endif