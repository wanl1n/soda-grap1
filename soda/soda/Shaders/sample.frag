#version 330 core

out vec4 FragColor;

// Texture of the object.
uniform sampler2D tex0;
uniform sampler2D norm_tex;
uniform int lightType; // 1 = point, 2 = direction

// Point Light Properties
uniform vec3 lightPos_p;
uniform vec3 lightColor_p;
uniform float dist_p;
uniform vec3 ambientColor_p;
uniform vec3 cameraPos_p;
uniform float intensityMult_p;

// Directional Light Properties
uniform vec3 lightPos_d;
uniform vec3 lightColor_d;
uniform float dist_d;
uniform vec3 ambientColor_d;
uniform vec3 cameraPos_d;
uniform float intensityMult_d;

// Common properties of the light sources.
uniform float ambientStr;
uniform float specStr;
uniform float specPhong;

// Coordinates of the texture and the object.
in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

// Calculate the lighting of the directional light on the color of the object/model.
vec4 calcDirLight() {
	// Get the normal coordinates
	vec3 normal = normalize(normCoord);

	// The intensity is simply based on the multiplier since directional light isn't dependent on distance.
	float intensity = intensityMult_d;
	// Calculate the direction of the light by getting the unit vector of its position from the center.
	vec3 lightDir = normalize(vec3(4, 11, -3));
	
	// Calculate the diffuse of the light based on which way it's from.
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor_d;

	// Calculate the ambient color based on the set ambient light strength.
	vec3 ambientCol = ambientColor_d * ambientStr;

	// Get the view direction of the camera to determine the location of the reflection.
	vec3 viewDir = normalize(cameraPos_d - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	// Calculate the specular light based on the view direction.
	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColor_d;

	// Return the resulting fragment texture with applied point light lighting.
	return vec4(intensity * (diffuse + ambientCol + specColor), 1.0);
}

// Calculate the lighting of the point light on the color of the object/model.
vec4 calcPointLight() {
	// Get the normal coordinates
	vec3 normal = normalize(normCoord);

	// Calculate the intensity of the light based on the distance of the light from the object.
	float intensity = 1 / (dist_p * dist_p) * intensityMult_p;

	// Calculate the direction of the light by getting the unit vector of its position minus the object.
	vec3 lightDir = normalize(lightPos_p - fragPos);
	
	// Calculate the diffuse of the light based on which way it's from.
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor_p;

	// Calculate the ambient color based on the set ambient light strength.
	vec3 ambientCol = ambientColor_p * ambientStr;

	// Get the view direction of the camera to determine the location of the reflection.
	vec3 viewDir = normalize(cameraPos_p - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	// Calculate the specular light based on the view direction.
	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColor_p;

	// Return the resulting fragment texture with applied point light lighting.
	return vec4(intensity * (diffuse + ambientCol + specColor), 1.0);
}

void main() {
	// Assign the texture color using the function
	// Light is additive so simply add both the lighting effects together in the final.
	FragColor += calcDirLight() * texture(tex0, texCoord);
	FragColor += calcPointLight() * texture(tex0, texCoord);
}