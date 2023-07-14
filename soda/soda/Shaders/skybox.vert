#version 330 core 

layout(location = 0) in vec3 aPos;

// vec2 is for 2D
// vec3 is for cubemaps
out vec3 texCoord;

uniform mat4 projection;
uniform mat4 view;

void main() {
	
	vec4 pos = projection * view * vec4(aPos, 1.0);

	//directly write onto view space
	gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);

	
	texCoord = aPos;
}