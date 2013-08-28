#version 130
#extension GL_EXT_gpu_shader4 : enable
uniform sampler2DArray texture;
uniform sampler2DArray tonemap;

uniform vec4 special;

in vec3 texCoord;
in vec4 colordata;

const vec3 degamma = vec3(1.0 / 2.2);
const vec3 gamma   = vec3(2.2);

void main(void) {
	
	vec4 color = texture2DArray(texture, texCoord);
	if (color.a < 0.1) discard; // le discard!
	
	// special color
	if (special.a > 0.5)
	{
		vec4 tone = texture2DArray(tonemap, texCoord);
		color.rgb = mix(color.rgb, tone.rgb * special.rgb, tone.a);
	}
	
	color.rgb = pow(color.rgb, degamma);
	
	color.rgb = mix(color.rgb, colordata.rgb, colordata.a);
	
	gl_FragColor = vec4(pow(color.rgb, gamma), color.a);
	
}
