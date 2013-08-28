#version 130
#extension GL_EXT_gpu_shader4 : enable
uniform sampler2DArray texture;
in vec4 texCoord;
in vec4 colordata;

const vec4 fontbg = vec4(0.0,0.0,0.0, 0.3);

void main(void) {

	vec4 color = texture2DArray(texture, texCoord.stp);
	
	float factor = color.r;
	color.a *= 1.0 - max(0.0, fontbg.a - factor);
	
	//if (color.a < 0.1) discard;
	
	gl_FragColor = color * colordata;

}
