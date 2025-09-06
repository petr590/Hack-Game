#version 330 core

#define GRAY(rgb, a) vec4(rgb, rgb, rgb, a)

uniform vec3 centerPos;
uniform float progress;
uniform int seed;

in vec3 vertexPos;

out vec4 result;

const float TILE_SIZE = 0.05;
const float END_TIME = 0.4;

const float SQUARE_ANGLE = radians(30.0);
const float SQUARE_ANGLE_SIN = sin(SQUARE_ANGLE);
const float SQUARE_ANGLE_COS = cos(SQUARE_ANGLE);

const float NOISE_START = 0.05;
const int NOISE_OUTER_RADIUS = 25;
const int NOISE_INNER_RADIUS = 22;


vec4 blend(vec4 color1, vec4 color2) {
	float alpha = color2.a + color1.a * (1.0 - color2.a);
	vec3 rgb = vec3(0.0);
	
	if (alpha > 0.0) {
		rgb = (color2.rgb * color2.a + color1.rgb * color1.a * (1.0 - color2.a)) / alpha;
	}
	
	return vec4(rgb, alpha);
}

float zoom(float value, float srcStart, float srcEnd, float dstStart, float dstEnd) {
	return mix(dstStart, dstEnd, (value - srcStart) / (srcEnd - srcStart));
}

vec2 rotate(vec2 vec, float sin_v, float cos_v) {
	return vec2(
		vec.x * cos_v - vec.y * sin_v,
		vec.x * sin_v + vec.y * cos_v
	);
}


const float NOISE_CHANCE = 0.25;

float simpleNoise(ivec2 pos) {
	int h = pos.x * 374761393 + pos.y * 668265263 + seed * 9871;
	h = (h ^ (h >> 13)) * 1274126177;
	h = h ^ (h >> 16);
	
	return h * (1.0 / 0x80000000u);
}


vec4 getNoise(vec2 pos) {
	pos *= zoom(progress, NOISE_START, END_TIME, 175.0, 150.0);
	ivec2 ipos = ivec2(pos);
	
	if (dot(ipos, ipos) > NOISE_OUTER_RADIUS * NOISE_OUTER_RADIUS) {
		return GRAY(0.0, 0.0);
	}
	
	float noise = simpleNoise(ipos);
	
	float alpha = noise > NOISE_CHANCE ? 0.0 :
			noise * (1.0 / NOISE_CHANCE) * zoom(progress, NOISE_START, END_TIME, 0.5, 0.0);
	
	return GRAY(0.0, alpha * smoothstep(NOISE_OUTER_RADIUS, NOISE_INNER_RADIUS, length(pos)));
}


void main() {
	
	if (progress >= END_TIME) {
		discard;
	}
	
	float alpha = max(0.0, mod(0.025 - progress, 0.1) - 0.05) * (5.5 - 20.0 * progress);
	
	vec2 pos = rotate(centerPos.xz - vertexPos.xz, SQUARE_ANGLE_SIN, SQUARE_ANGLE_COS);
	vec2 dist = abs(pos);
	float size = 1.0 * progress + 0.1;
	
	if (dist.x <= size && dist.y <= size) {
		result = GRAY(0.0, alpha);
		
		if (progress > NOISE_START) {
			result = blend(result, getNoise(pos));
		}
		
		return;
	}
	
	float rectSize = size * 2.0;
	
	if (dist.x <= size && dist.y <= rectSize ||
		dist.x <= rectSize && dist.y <= size) {
		
		result = GRAY(0.0, alpha * 0.5);
		return;
	}
	
	discard;
}
