#version 330 core

uniform sampler2D screenTexture;
uniform float progress;
uniform vec2 pixelSize;

in vec2 fragTexCoord;

out vec4 color;

const float MAX_FADE = 0.6;
const float MARGIN_MAX_FADE = 0.3;
const float MARGIN_HEIGHT = 0.1;
const float EPSILON = 1e-3;

void main() {
	float upperMaxFade = smoothstep(      MARGIN_HEIGHT,       MARGIN_HEIGHT + EPSILON, fragTexCoord.y);
	float lowerMaxFade = smoothstep(1.0 - MARGIN_HEIGHT, 1.0 - MARGIN_HEIGHT - EPSILON, fragTexCoord.y);
	float maxFade = mix(MARGIN_MAX_FADE, MAX_FADE, upperMaxFade * lowerMaxFade);
	
	vec4 color1 = texture(screenTexture, fragTexCoord);
	vec4 color2 = texture(screenTexture, fragTexCoord + pixelSize);
	color = vec4(color1.r, color2.g, color1.b, color1.a) * mix(1.0, maxFade, progress);
}
