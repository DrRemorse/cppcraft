#version 130
#extension GL_EXT_gpu_shader4 : enable
uniform sampler2DArray texture;
uniform sampler2DArray tonemap;

uniform float daylight;
uniform vec4 colordata;
uniform vec4 special;

in vec3 texCoord;
in float worldLight;
in float brightness;

void main(void)
{	
	vec4 color = texture2DArray(texture, texCoord);
	if (color.a < 0.1) { discard; } // le discard!
	
	// special color
	if (special.a > 0.5)
	{
		vec4 tone = texture2DArray(tonemap, texCoord);
		color.rgb = mix(color.rgb, tone.rgb * special.rgb, tone.a);
	}
	
	#include "degamma.glsl"
	
	// shadows & torchlight
	color.rgb *= worldLight * min(1.0, daylight + brightness);
	color.rgb = mix(color.rgb, colordata.rgb, max(0.0, colordata.a - brightness));
	
	#include "finalcolor.glsl"
}
