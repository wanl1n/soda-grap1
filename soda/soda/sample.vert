#version 330 core

//location = 0 is from vertexattributepointer first argument
layout(location = 0) in vec3 aPos;

void main() {
	gl_Position = vec4(aPos, 1.0);
}