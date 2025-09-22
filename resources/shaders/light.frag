#version 330 core

uniform vec3 modelColor;
uniform float modelBrightness;

out vec4 color;

void main() {
	color = vec4(modelColor * modelBrightness, 1.0f);
}