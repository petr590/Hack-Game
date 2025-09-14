#version 330 core

uniform vec3 centerPos;
uniform float progress;

in vec3 vertexPos;

out vec4 color;

const float TILE_SIZE = 0.05;
const float WIDTH_OFFSET = 0.15;
const float WIDTH_PERIOD = 0.75;
const float EPSILON = 0.003;

void main() {
	vec3 diff = centerPos - vertexPos;
	float dist = length(vec3(diff));
	
	float width = (mod(progress - WIDTH_OFFSET, WIDTH_PERIOD) + 0.01) * 0.012;
	
	float outer = (progress + 1.0) * TILE_SIZE * 0.5;
	float inner = outer - width;
	
	if (dist > outer || dist < inner) {
		discard;
	}
	
	float alphaOuter = smoothstep(outer, outer - EPSILON, dist);
	float alphaInner = smoothstep(inner, inner + EPSILON, dist);
	float alpha = sqrt(alphaOuter * alphaInner);
	
	if (alpha < 0.01) discard;
	
	color.rgb = vec3(1.0);
	color.a = alpha;
}
