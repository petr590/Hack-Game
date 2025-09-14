#version 330 core

uniform vec3 modelColor;

out vec4 color;

void main() {
	color = vec4(modelColor, 1.0f);
}