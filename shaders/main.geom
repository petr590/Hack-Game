#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vertexPos[];

out vec3 fragmentPos;
flat out vec3 normal;

void main() {
	vec3 edge1 = vertexPos[1] - vertexPos[0];
	vec3 edge2 = vertexPos[2] - vertexPos[0];
	normal = normalize(cross(edge1, edge2));

	for (int i = 0; i < 3; i++) {
		fragmentPos = vertexPos[i];
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}