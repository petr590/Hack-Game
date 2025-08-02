#version 330 core

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 modelColor;

in vec3 vertexPos;
in vec3 vertexNormal;

out vec4 color;


const float AMBIENT = 0.1;

void main() {
	vec3 lightDir = normalize(lightPos - vertexPos);
	
	float diffuse = max(dot(vertexNormal, lightDir), 0.0);

	vec3 result = (AMBIENT + diffuse) * lightColor * modelColor;
	color = vec4(result, 1.0f);
}