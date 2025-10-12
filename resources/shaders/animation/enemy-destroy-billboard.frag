uniform vec3 centerPos;
uniform float progress;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec3 fragPos;
in vec2 fragTexCoord;

out vec4 result;


const float TILE_SIZE = 0.05;

const float SMALL_SHADE_WIDTH = 0.0015;
const float BIG_SHADE_WIDTH = 0.05;

const float EXPLOSION_START = 0.03;
const float EXPLOSION_END   = 0.1;

const float REST_START = 0.1;
const float REST_END   = 0.3;

const float FIG0_START = 0.15;
const float FIG0_END   = 0.2;

const float FIG1_START = 0.25;
const float FIG1_END   = 0.3;

const float FIG2_START = 0.35;
const float FIG2_END   = 0.4;

const float FIG2_ANGLE = radians(45.0);
const float FIG2_ANGLE_SIN = sin(FIG2_ANGLE);
const float FIG2_ANGLE_COS = cos(FIG2_ANGLE);


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
	
	if (color.a >= 0.01) {
		result = blend(result, color);
	}
}


void drawFigure(sampler2D tex, float angleSin, float angleCos) {
	vec2 scaledTexCoord = rotate(fragTexCoord - 0.5, angleSin, angleCos) * 200.0 + 0.5;
	result = blend(result, texture(tex, scaledTexCoord));
}


void main() {
	result = vec4(0.0);
	float dist = distance(fragPos, centerPos);
	
	if (progress >= EXPLOSION_START && progress <= EXPLOSION_END) {
		float alpha = min(1.0, zoom(progress, EXPLOSION_START, EXPLOSION_END, 5.0, 0.0));
		
		drawCircle(dist, EXPLOSION_START, EXPLOSION_END, 2.0 * TILE_SIZE, 4 * TILE_SIZE, BIG_SHADE_WIDTH, GRAY(0.0, alpha * 0.2));
		drawCircle(dist, EXPLOSION_START, EXPLOSION_END, 1.0 * TILE_SIZE, 3 * TILE_SIZE, BIG_SHADE_WIDTH, GRAY(1.0, alpha * 0.5));
		drawCircle(dist, EXPLOSION_START, EXPLOSION_END, 0.6 * TILE_SIZE, 2 * TILE_SIZE, BIG_SHADE_WIDTH, GRAY(1.0, alpha));
	}
	
	if (progress >= REST_START && progress <= REST_END) {
		float alpha = zoom(progress, REST_START, REST_END, 0.5, 0.0);
		
		drawCircle(dist, REST_START, REST_END, 0.8 * TILE_SIZE, 2 * TILE_SIZE, BIG_SHADE_WIDTH, GRAY(1.0, alpha));
		drawCircle(dist, REST_START, REST_END, 0.0, 0.0, BIG_SHADE_WIDTH, vec4(1.0, 0.5, 0.5, alpha));
	}
	
	
	if (progress >= FIG0_START && progress <= FIG0_END) {
		drawFigure(texture0, 0.0, 1.0);
		
	} else if (progress >= FIG1_START && progress <= FIG1_END) {
		drawFigure(texture1, 0.0, 1.0);
		
	} else if (progress >= FIG2_START && progress <= FIG2_END) {
		drawFigure(texture2, FIG2_ANGLE_SIN, FIG2_ANGLE_COS);
	}
	
	
	if (drawRing(dist, 0.03, 0.1,  1.5 * TILE_SIZE,  3 * TILE_SIZE, 0.003, SMALL_SHADE_WIDTH, vec3(0.0))) return;
	if (drawRing(dist, 0.06, 0.33, 4.0 * TILE_SIZE, 15 * TILE_SIZE, 0.006, SMALL_SHADE_WIDTH, vec3(0.0))) return;
}
