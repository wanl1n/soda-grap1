#version 330 core

out vec4 FragColor;

uniform float r;
uniform float g;
uniform float b;

void main() {
	FragColor = vec4(1.0f + r, 1.0f + g, 1.0f + b, 1.0f); //rgba
}