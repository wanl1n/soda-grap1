#include "TexturedModel.h"

using namespace models;

TexturedModel::TexturedModel(std::string path, glm::vec3 pos, glm::vec3 scale,
                            GLuint* shaderProgram,
                            const char* texturePath,
                            GLuint texture_ind) : Model(pos, scale) {
    loadModelData(path);
    loadTexture(texturePath, texture_ind);
    this->textureIndex = texture_ind;
    this->shaderProgram = shaderProgram;
}

void TexturedModel::loadModelData(std::string path) {

    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shape,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shape[0].mesh.indices[i];

        this->fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);// X
        this->fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);// Y
        this->fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);// Z

        this->fullVertexData.push_back(attributes.normals[vData.normal_index * 3]);// X
        this->fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 1]);// Y
        this->fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 2]);// Z

        this->fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);
        this->fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);
    }

    generateBuffers();
}

void TexturedModel::generateBuffers() {

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);

    GLintptr normPtr = 3 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)normPtr);

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)uvPtr);


    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(1); // for Normals
    glEnableVertexAttribArray(2); // 2 for UV / Texture

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Wala nang ginagalaw sa VBO.
    glBindVertexArray(0); // Wala ka nang ginagalaw na VAO.
}

void TexturedModel::loadTexture(const char* path, GLuint texture_ind) {
    int img_width, img_height, color_channels; // Width, Height, and color channels of the Texture.

    // Fix the flipped texture (by default it is flipped).
    stbi_set_flip_vertically_on_load(true);
    // Load the texture and fill out the variables.
    unsigned char* text_bytes = stbi_load(path, // Texture path
        &img_width, // Width of the texture
        &img_height, // height of the texture
        &color_channels, // color channel
        0);

    // Generate a reference.
    glGenTextures(1, &texture);
    // Set the current texture we're working on to Texture 0.
    glActiveTexture(texture_ind);
    // Bind our next tasks to Tex0 to our current reference similar to VBOs.
    glBindTexture(GL_TEXTURE_2D, texture);
    //If you want to set how the texture maps on a different size model
    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S, // XY = ST (s for x, t for y)
        GL_CLAMP_TO_EDGE //GL_CLAMP_TO_EDGE for stretch, 
    );
    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T, // XY = ST (s for x, t for y)
        GL_REPEAT //GL_CLAMP_TO_EDGE for stretch, 
    );

    unsigned int rgb = GL_RGB;
    if (color_channels == 3) {
        rgb = GL_RGB;
    }
    else if (color_channels == 4) {
        rgb = GL_RGBA;
    }

    //Assign the loaded texture to the OpenGL reference.
    glTexImage2D(
        GL_TEXTURE_2D,
        0, // Texture 0
        rgb, // Target color format of the texture.
        img_width, // Texture width
        img_height, // Texture height
        0,
        rgb, // Color format of the texture
        GL_UNSIGNED_BYTE,
        text_bytes // loaded texture in bytes
    );

    // Generate the mipmaps to the current texture
    glGenerateMipmap(GL_TEXTURE_2D);
    // Free up the loaded bytes.
    stbi_image_free(text_bytes);
}

void TexturedModel::draw(glm::mat4 projection, glm::mat4 viewMatrix) {

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

    glActiveTexture(this->textureIndex);
    GLuint tex0Address = glGetUniformLocation(*shaderProgram, "tex0");
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(tex0Address, 0);

    glBindVertexArray(this->VAO);

    // Draw the object (XYZUV in arrays) with texture.
    glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 5);
}

void TexturedModel::deleteBuffers() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}