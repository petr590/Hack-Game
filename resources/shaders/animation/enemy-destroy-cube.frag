#version 330 core

#define GRAY(rgb, a) vec4(rgb, rgb, rgb, a)

uniform float progress;
uniform uint mode;

in vec3 vertexPos;

out vec4 result;

const uint MODE_FADING = 0u;
const uint MODE_SOLID  = 1u;

const float LN_2 = 0.693147180559945309; // ln(2);


void main() {
	switch (mode) {
		case MODE_FADING:
			float alpha = exp(-progress * 9.0 * LN_2);
			result = GRAY(0.0, alpha);
			break;
		
		case MODE_SOLID:
			result = GRAY(0.0, 1.0);
			break;
	}
}
