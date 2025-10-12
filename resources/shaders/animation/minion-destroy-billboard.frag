uniform vec3 centerPos;
uniform vec3 angleNormal;
uniform float progress;
uniform int seed;

in vec3 fragPos;

out vec4 result;

const float EPSILON = 0.0015;

const float RING_END        = 0.5;
const float RING_MAX_RADIUS = 0.16;

const float LINE_MIN_LENGHT = 0.1;
const float LINE_MAX_LENGHT = 0.25;
const int LINE_COUNT = 6;

const float PI = radians(180.0);

float randomBetween(float low, float high, int localSeed) {
	return mix(low, high, simpleNoise(seed, localSeed));
}


void drawRing(float dist) {
	if (progress > RING_END) return;
	
	float time = progress * (1.0 / RING_END);
	float radius = time * RING_MAX_RADIUS;
	
	float outer = smoothstep(radius + EPSILON, radius, dist);
	float inner = smoothstep(radius - EPSILON, radius, dist);
	float alpha = smoothstep(0.5, 1.0, time);
	
	result = blend(result, GRAY(0.0, outer * inner * alpha));
}

void drawLine(float dist, float angle, float arcLen, int localSeed) {
	float len = randomBetween(LINE_MIN_LENGHT, LINE_MAX_LENGHT, localSeed);
	float lineStart = progress * randomBetween(0.5, 1.0, localSeed + 1);
	float lineEnd   = lineStart + len;
	
	if (dist < lineStart || dist > lineEnd) return;
	
	float up    = smoothstep(lineEnd, lineEnd - EPSILON, dist);
	float down  = smoothstep(lineStart - EPSILON, lineStart, dist);
	float left  = smoothstep( EPSILON, 0.0, arcLen);
	float right = smoothstep(-EPSILON, 0.0, arcLen);
	
	result = blend(result, GRAY(0.0, up * down * left * right));
}


void drawLines(float dist) {
	vec3 direction = normalize(fragPos - centerPos);
	float angle = acos(dot(angleNormal, direction)) * sign(angleNormal.x - direction.x);
	
	int index = -1;
	float arcLen = 0.0;
	
	for (int i = 0; i < LINE_COUNT; i++) {
		arcLen = (angle - randomBetween(-PI, PI, i)) * dist;
		
		if (arcLen >= -EPSILON && arcLen <= EPSILON) {
			index = i;
			break;
		}
	}
	
	if (index >= 0) {
		drawLine(dist, angle, arcLen, index + LINE_COUNT);
	}
}


void main() {
	result = vec4(0.0);
	
	float dist = distance(fragPos, centerPos);
	drawRing(dist);
	drawLines(dist);
}
