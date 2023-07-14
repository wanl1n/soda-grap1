#version 330 core

//location = 0 is from VertexAttributePointer first argument
layout(location = 0) in vec3 aPos;

//The Tex coord / UV is at 0
layout(location = 2) in vec2 aTex;

//The normals has attribute position 1
//Accesses the normals and assigns it to vertexNormal.
layout(location = 1) in vec3 vertexNormal;

//Pass the tex coord to the fragment shader.
out vec2 texCoord;

//Pass the processed normals to the fragment shder later
out vec3 normCoord;
//Pass the position of the vertex to the fragment shader later
out vec3 fragPos;

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

	// Get the Normal Matrix and convert it to a 3x3 matrix
	// Apply the normal matrix to the normal data.
	normCoord = mat3(transpose(inverse(transform))) * vertexNormal;

	//The position is just your transformation matrix
	//applied to the vertex as a vector 3
	fragPos = vec3(transform * vec4(aPos, 1.0));
}