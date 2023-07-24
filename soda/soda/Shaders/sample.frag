#version 330 core

out vec4 FragColor;

uniform sampler2D tex0;
uniform sampler2D norm_tex;
uniform int lightType; // 1 = point, 2 = direction

// Point Light
uniform vec3 lightPos_p;
uniform vec3 lightColor_p;
uniform float dist_p;
uniform vec3 ambientColor_p;
uniform vec3 cameraPos_p;
uniform float intensityMult_p;

// Point Light
uniform vec3 lightPos_d;
uniform vec3 lightColor_d;
uniform float dist_d;
uniform vec3 ambientColor_d;
uniform vec3 cameraPos_d;
uniform float intensityMult_d;

uniform float ambientStr;
uniform float specStr;
uniform float specPhong;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

vec4 calcDirLight() {
	vec3 normal = normalize(normCoord);

	float intensity = intensityMult_d;
	vec3 lightDir = normalize(vec3(4, 11, -3));
	
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor_d;

	vec3 ambientCol = ambientColor_d * ambientStr;

	vec3 viewDir = normalize(cameraPos_d - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	//Get the specular light
	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColor_d;

	return vec4(intensity * (diffuse + ambientCol + specColor), 1.0);
}

vec4 calcPointLight() {
	vec3 normal = normalize(normCoord);

	float intensity = 1 / (dist_p * dist_p) * intensityMult_p;
	vec3 lightDir = normalize(lightPos_p - fragPos);
	
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor_p;

	vec3 ambientCol = ambientColor_p * ambientStr;

	vec3 viewDir = normalize(cameraPos_p - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColor_p;

	return vec4(intensity * (diffuse + ambientCol + specColor), 1.0);
}

void main() {
	// Assign the texture color using the function
	FragColor += calcDirLight() * texture(tex0, texCoord);
	FragColor += calcPointLight() * texture(tex0, texCoord);
}