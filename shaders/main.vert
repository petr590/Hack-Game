#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexPos;
out vec3 vertexNormal;

void main() {
	vertexNormal = normal;

	vec4 pos = model * vec4(position, 1.0);
	vertexPos = vec3(pos);
	gl_Position = projection * view * pos;
}