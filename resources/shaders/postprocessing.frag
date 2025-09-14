#version 330 core

uniform sampler2D screenTexture;
uniform float progress;

in vec2 vertexTexCoords;

out vec4 color;

const float MAX_FADE = 0.6;
const float MARGIN_MAX_FADE = 0.3;
const float MARGIN_HEIGHT = 0.1;
const float EPSILON = 1e-3;

void main() {
	float upperMaxFade = smoothstep(      MARGIN_HEIGHT,       MARGIN_HEIGHT + EPSILON, vertexTexCoords.y);
	float lowerMaxFade = smoothstep(1.0 - MARGIN_HEIGHT, 1.0 - MARGIN_HEIGHT - EPSILON, vertexTexCoords.y);
	float maxFade = mix(MARGIN_MAX_FADE, MAX_FADE, upperMaxFade * lowerMaxFade);
	color = texture(screenTexture, vertexTexCoords) * mix(1.0, maxFade, progress);
}
