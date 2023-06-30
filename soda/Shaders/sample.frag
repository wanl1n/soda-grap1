#version 330 core

// Point Light
out vec4 FragColor;

// For changing r g b values
uniform float r;
uniform float g;
uniform float b;

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

// Distance of object from light
uniform float dist;
// Intensity multiplier; Taken as input from main and can be any float number.
// A higher number makes the light brighter
uniform float intensityMult;

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
	vec3 lightDir = normalize(lightPos - fragPos);

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

	//Intensity of the point light
	// Using the formula for intensity, 1/distance^2, we get the attenuation. The further it gets, the smaller the value of the intensity 1, 1/4, 1/6, and so on.
	// Multiply this by the multiplier, this can be any float value that will make the light brighter as it goes higher.
	float intensity = 1 / (dist * dist) * intensityMult;

	//Changing the color using rgba values.
	//FragColor = vec4(1.0f + r, 0.75f + g, 0.796f + b, 1.0f); //rgba

	// Assign the texture color using the function
	// Apply the light * intensity
	// Multiply the light and texture with the intensity to make the light dimmer or brighter depending on where it is and how much the multiplier was set.
	FragColor = intensity * vec4(diffuse + ambientCol + specColor, 1.0) * texture(tex0, texCoord);
}