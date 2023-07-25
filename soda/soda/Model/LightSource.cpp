#include "LightSource.h"

using namespace models;

// This is a light source model.
LightSource::LightSource(std::string path, glm::vec3 pos, glm::vec3 scale,
                        GLuint* shaderProgram) : Model(pos, scale), light(pos) {
    // Initializes a light that is positioned in the same point in space.

    // Sets the model color to white.
    this->modelColor = glm::vec4(1, 1, 1, 1);
    // Loads the model data (indices, VAOs, VBOs, EBOs)
    this->loadModelData(path);

    // sets the shader program of this model to the passed shader program.
    this->shaderProgram = shaderProgram;

    // Sets the radius to 5 (this is the distance of the light source model from the main model.
    this->radius = 5.f;
}

// Loads the data of the model taken from the path.
void LightSource::loadModelData(std::string path) {
    
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shape,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    // gets the indices of the model.
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        this->mesh_indices.push_back(shape[0].mesh.indices[i].vertex_index);
    }

    // Generate buffers for the vertices of the model.
    this->generateBuffers();
}

// Generates the buffers of the object.
void LightSource::generateBuffers() {

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Binds VBO to VAO.
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * attributes.vertices.size(),
        &attributes.vertices[0],
        GL_STATIC_DRAW //GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Wala nang ginagalaw sa VBO.
    glBindVertexArray(0); // Wala ka nang ginagalaw na VAO.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // EBO
}

void LightSource::draw(glm::mat4 projection, glm::mat4 viewMatrix) {

    // Set the shaderprogram to be used.
    glUseProgram(*this->shaderProgram);

    // create a transformation matrix with the position of the object
    glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0f),
        this->pos);

    // Scales the object based on the set scale.
    transformation_matrix = glm::scale(transformation_matrix,
        this->scale);

    // Rotates the object about the x-axis.
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->thetaX),
        this->xAxis);
    // Rotates the object about the y-axis.
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->thetaY),
        this->yAxis);
    // Rotates the object about the z-axis.
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->thetaZ),
        this->zAxis);

    // Updates the position of the light so it matches the light source model.
    this->light.setPosition(this->pos);

    // Update the shader with the new transform of the object.
    unsigned int transformLoc = glGetUniformLocation(*shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

    // Update the new projection of the camera.
    unsigned int projLoc = glGetUniformLocation(*shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Update the view matrix of the camera.
    unsigned int viewLoc = glGetUniformLocation(*shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Update the color of the model for when it's being selected or unselected.
    unsigned int colorLoc = glGetUniformLocation(*shaderProgram, "color");
    glUniform4fv(colorLoc, 1, glm::value_ptr(this->modelColor));

    // Bind this object's VAO before drawing.
    glBindVertexArray(this->VAO);

    // Draw the object (XYZUV in arrays) using the indices.
    glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
}

// Used to rotate the light source object around the main object.
void LightSource::rotate(float thetaX, float thetaY, float thetaZ) {

    // updates the rotation of the light source object by the offset.
    this->thetaX += thetaX;
    this->thetaY += thetaY;
    this->thetaZ += thetaZ;

    // Need to calculate the new position of the object to update the position property
    // Calculate the rotation axes calculated using the mouse position and the angle limit of the screen.
    float yaw = glm::radians((this->thetaX / radius) * 359.f);
    float pitch = glm::radians((this->thetaY / radius) * 359.f);

    // Limiting the degree in case of flipping.
    if (yaw > 359.9f) yaw = 359.9f;
    if (yaw < -359.9f) yaw = -359.9f;
    if (pitch > 359.9f) pitch = 359.9f;
    if (pitch < -359.9f) pitch = -359.9f;

    // Finally get the direction in each axis by using Polar to Cartesian point conversion.
    float xAxisRot = this->radius * sin(yaw) * cos(pitch);
    float yAxisRot = this->radius * sin(pitch);
    float zAxisRot = this->radius * cos(yaw) * cos(pitch);

    // Update the model position with the new calculated point.
    this->pos = glm::vec3(xAxisRot, yAxisRot, zAxisRot);
}

// Delete the buffer for clean up.
void LightSource::deleteBuffers() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// Returns the reference of the light.
PointLight* LightSource::getLight() {
    return &this->light;
}

// Set the color of this model to the accepted color and update the color of the light as well.
void LightSource::setColor(glm::vec4 color) {
    this->modelColor = color;

    this->light.setColor(this->modelColor);
}