#version 330 core

out vec4 FragColor;

in vec3 texCoord;

// add in a cubemap for later.
uniform samplerCube skybox;

void main() {
	FragColor = texture(skybox, texCoord);
}