#version 330 core

// This is the shader program of the light source model.
// Color of the model.
out vec4 FragColor;

// Accepts the color of the model as input.
uniform vec4 color;

void main() {
	 
	// If no color was set, this is the default
	FragColor = vec4(0.5f, 1.0f, 1.0f, 1.0f); //rgba;

	// Sets the color of the model to the accepted color.
	FragColor = color;
}