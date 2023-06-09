#version 330 core

out vec4 FragColor;

uniform float r;
uniform float g;
uniform float b;

// Texture to be passed
uniform sampler2D tex0;

// Should receive the texCoord from the vertex shader.
in vec2 texCoord;

void main() {
	//FragColor = vec4(1.0f + r, 0.75f + g, 0.796f + b, 1.0f); //rgba

	// Assign the texture color using the function
	FragColor = texture(tex0, texCoord);
}