#version 330 core

//location = 0 is from VertexAttributePointer first argument
layout(location = 0) in vec3 aPos;

//The Tex coord / UV is at 0
layout(location = 2) in vec2 aTex;

//Pass the tex coord to the fragment shader.
out vec2 texCoord;

// Creates a Transform variable.
// We'll assign the transformation matrix here later.
uniform mat4 transform;

// Creates the View Matrix variable.
uniform mat4 view;

// Creates a Projection variable.
// We'll assign the Projection matrix here.
uniform mat4 projection;


void main() {
	// Multiply the transformation matrix to the vec4 version of aPos to get the final position.
	gl_Position = projection * view * transform * vec4(aPos, 1.0);

	//Assigns the UV
	texCoord = aTex;
}