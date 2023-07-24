#version 330 core

out vec4 FragColor;

uniform vec4 color;

void main() {
	 
	FragColor = vec4(0.5f, 1.0f, 1.0f, 1.0f); //rgba;
	FragColor = color;
}