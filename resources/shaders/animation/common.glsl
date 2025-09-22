#version 330 core

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

vec2 rotate(vec2 vec, float angleSin, float angleCos) {
	return vec2(
		vec.x * angleCos - vec.y * angleSin,
		vec.x * angleSin + vec.y * angleCos
	);
}
