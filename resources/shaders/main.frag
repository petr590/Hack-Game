#version 330 core

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 modelColor;
uniform float modelBrightness;

in vec3 fragPos;
in vec3 fragNormal;

out vec4 color;


const float AMBIENT = 0.54;

void main() {
	vec3 lightDir = normalize(lightPos - fragPos);
	
	float diffuse = max(dot(fragNormal, lightDir), 0.0) * (1.0 - AMBIENT);

	vec3 result = (AMBIENT + diffuse) * lightColor * modelColor * modelBrightness;
	color = vec4(result, 1.0f);
}
