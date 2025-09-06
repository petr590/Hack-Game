#version 330 core

#define GRAY(rgb, a) vec4(rgb, rgb, rgb, a)

uniform vec3 centerPos;
uniform float progress;

in vec3 vertexPos;

out vec4 result;


const float TILE_SIZE = 0.05;

const float SMALL_SHADE_WIDTH = 0.0015;
const float BIG_SHADE_WIDTH = 0.05;

const float EXPLOSION_START = 0.03;
const float EXPLOSION_END   = 0.1;

const float REST_START = 0.1;
const float REST_END   = 0.3;

const float FIG1_START = 0.15;
const float FIG1_END = 0.15;


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


bool drawRing(float dist, float startTime, float endTime, float minRadius, float maxRadius, float width, float shadeWidth, vec3 rgb) {
	if (progress < startTime || progress > endTime) {
		return false;
	}

	float innerRadius = zoom(progress, startTime, endTime, minRadius, maxRadius);
	float outerRadius = innerRadius + width;
	
	if (dist < innerRadius - shadeWidth || dist > outerRadius + shadeWidth) {
		return false;
	}
	
	float alphaInner = smoothstep(innerRadius, innerRadius + shadeWidth, dist);
	float alphaOuter = smoothstep(outerRadius, outerRadius - shadeWidth, dist);
	float alpha = alphaOuter * alphaInner;
	
	if (alpha < 0.01) {
		return false;
	}
	
	result = blend(result, vec4(rgb, alpha));
	return true;
}


void drawCircle(float dist, float startTime, float endTime, float minRadius, float maxRadius, float shadeWidth, vec4 color) {
	float radius = zoom(progress, startTime, endTime, minRadius, maxRadius);
	
	if (dist > radius + shadeWidth) {
		return;
	}
	
	color.a *= smoothstep(radius + shadeWidth, radius, dist);
	
	if (color.a < 0.01) {
		return;
	}
	
	result = blend(result, color);
}


void drawFig1() {
//	vec2 diff = centerPos.xz - vertexPos.xz;
	// TODO
}


void main() {
	result = vec4(0.0);
	float dist = length(centerPos.xz - vertexPos.xz);
	
	if (progress >= EXPLOSION_START && progress <= EXPLOSION_END) {
		float alpha = min(1.0, zoom(progress, EXPLOSION_START, EXPLOSION_END, 5.0, 0.0));
		
		drawCircle(dist, EXPLOSION_START, EXPLOSION_END, 2.0 * TILE_SIZE, 4 * TILE_SIZE, BIG_SHADE_WIDTH, GRAY(0.0, alpha * 0.2));
		drawCircle(dist, EXPLOSION_START, EXPLOSION_END, 1.0 * TILE_SIZE, 3 * TILE_SIZE, BIG_SHADE_WIDTH, GRAY(1.0, alpha * 0.5));
		drawCircle(dist, EXPLOSION_START, EXPLOSION_END, 0.6 * TILE_SIZE, 2 * TILE_SIZE, BIG_SHADE_WIDTH, GRAY(1.0, alpha));
	}
	
	if (progress >= FIG1_START && progress <= FIG1_END) {
		
	}
	
	
	if (progress >= REST_START && progress <= REST_END) {
		float alpha = zoom(progress, REST_START, REST_END, 0.5, 0.0);
		
		drawCircle(dist, REST_START, REST_END, 0.8 * TILE_SIZE, 2 * TILE_SIZE, BIG_SHADE_WIDTH, GRAY(1.0, alpha));
		drawCircle(dist, REST_START, REST_END, 0.0, 0.0, BIG_SHADE_WIDTH, vec4(1.0, 0.5, 0.5, alpha));
	}
	
	
	if (drawRing(dist, 0.03, 0.1,  1.5 * TILE_SIZE,  3 * TILE_SIZE, 0.003, SMALL_SHADE_WIDTH, vec3(0.0))) return;
	if (drawRing(dist, 0.06, 0.33, 4.0 * TILE_SIZE, 15 * TILE_SIZE, 0.006, SMALL_SHADE_WIDTH, vec3(0.0))) return;
}
