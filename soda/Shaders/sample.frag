#version 330 core

// Direcitonal Light
out vec4 FragColor;

// Texture to be passed
uniform sampler2D tex0;

//Position of the light source
uniform vec3 lightPos;
//Color of the light; rgb
uniform vec3 lightColor;
//Strength of the ambient light
uniform float ambientStr;
//Color of the ambient light
uniform vec3 ambientColor;

//Camera position
uniform vec3 cameraPos;
//Specular Str
uniform float specStr;
//Specular Phong
uniform float specPhong;

// Should receive the texCoord from the vertex shader.
in vec2 texCoord;
//Pass the processed normals to the fragment shder later
in vec3 normCoord;
//Pass the position of the vertex to the fragment shader later
in vec3 fragPos;

void main() {
	//Normalize the received normals
	vec3 normal = normalize(normCoord);
	//Get the direction of the light to the fragment
	vec3 lightDir = vec3(-1, -1, 0);

	//Apply the diffuse formula here
	float diff = max(dot(normal, lightDir), 0.0);
	//Multiply it to the desired light color.
	vec3 diffuse = diff * lightColor;

	//Get the ambient light
	vec3 ambientCol = ambientColor * ambientStr;

	//Get our view direction from the camera to the fragment
	vec3 viewDir = normalize(cameraPos - fragPos);
	//Get the reflection vector
	vec3 reflectDir = reflect(-lightDir, normal);

	//Get the specular light
	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColor;

	// Assign the texture color using the function
	FragColor = vec4(diffuse + ambientCol + specColor, 1.0) * texture(tex0, texCoord);
}