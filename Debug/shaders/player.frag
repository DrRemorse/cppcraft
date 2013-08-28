#version 130
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DArray texture;

uniform float daylight;
uniform vec4 colordata;

in vec3 texCoord;
flat in float worldLight;
in float brightness;

void main(void) {
	
	vec3 color = texture2DArray(texture, texCoord).rgb;
	
	#include "degamma.glsl"
	
	// shadows & torchlight
	color.rgb *= worldLight * min(1.0, daylight + brightness);
	color.rgb = mix(color.rgb, colordata.rgb, max(0.0, colordata.a - brightness));
	
	#include "finalcolor.glsl"
}
