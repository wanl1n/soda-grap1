#include "LightSource.h"

using namespace models;

LightSource::LightSource(std::string path, glm::vec3 pos, glm::vec3 scale,
                        GLuint* shaderProgram) : Model(pos, scale), light(pos) {
    this->modelColor = glm::vec4(1, 1, 1, 1);
    this->loadModelData(path);

    this->shaderProgram = shaderProgram;
}

void LightSource::loadModelData(std::string path) {
    // Banana Cat Object elements
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

    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        this->mesh_indices.push_back(shape[0].mesh.indices[i].vertex_index);
    }

    this->generateBuffers();
}


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

    // Get the shaderprogram to be used.
    glUseProgram(*this->shaderProgram);

    // Create the transformation matrix and apply the transformation attributes at draw.
    glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0f),
        this->pos);

    transformation_matrix = glm::scale(transformation_matrix,
        this->scale);

    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->thetaX),
        this->xAxis);
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->thetaY),
        this->yAxis);
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->thetaZ),
        this->zAxis);

    // Update the shader with the new transform of the object.
    unsigned int transformLoc = glGetUniformLocation(*shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

    unsigned int projLoc = glGetUniformLocation(*shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    unsigned int viewLoc = glGetUniformLocation(*shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    unsigned int colorLoc = glGetUniformLocation(*shaderProgram, "color");
    glUniform4fv(colorLoc, 1, glm::value_ptr(this->modelColor));

    glBindVertexArray(this->VAO);

    // Draw the object (XYZUV in arrays) with texture.
    glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
}

void LightSource::deleteBuffers() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

PointLight* LightSource::getLight() {
    return &this->light;
}

void LightSource::setColor(glm::vec4 color) {
    this->modelColor = color;

    this->light.setColor(this->modelColor);
}