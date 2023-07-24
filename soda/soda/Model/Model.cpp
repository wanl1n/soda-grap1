#include "Model.h"

using namespace models;

Model::Model(glm::vec3 pos, glm::vec3 scale) {

    this->pos = pos; // Take parameter as position.

    // Since model is initially flipped (facing backwards), flip the model.
    this->xAxis = glm::vec3(1, 0, 0); // Set axis of rotation to y-axis.
    this->yAxis = glm::vec3(0, 1, 0); // Set axis of rotation to y-axis.
    this->zAxis = glm::vec3(0, 0, 1); // Set axis of rotation to y-axis.
    this->thetaX = 0; // Rotate around y-axis by 180 degrees to flip the model.
    this->thetaY = 180.f; // Rotate around y-axis by 180 degrees to flip the model.
    this->thetaZ = 0; // Rotate around y-axis by 180 degrees to flip the model.

    // Since model is too big, scale it down to 0.1f.
    this->scale = scale;
}

void Model::rotate(float thetaX, float thetaY, float thetaZ) {
    this->thetaX += thetaX;
    this->thetaY += thetaY;
    this->thetaZ += thetaZ;
}