#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec3 vertexPos;
out vec2 vertexTexCoord;

void main() {
	vec4 pos = model * vec4(position, 1.0);
	vertexPos = vec3(pos);
	gl_Position = projection * view * pos;
	
	vertexTexCoord = texCoord;
}
