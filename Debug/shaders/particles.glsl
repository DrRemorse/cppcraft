#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM
precision mediump float;
#define POSTPROCESS

#ifdef VERTEX_PROGRAM
uniform mat4 matproj;
uniform mat4 matview;

uniform vec2 sizemult;

in vec3 in_vertex;
in vec2 in_data;
in vec2 in_normdata;
in vec4 in_color;

flat out float tileID;
out vec2 out_normdata;
flat out vec4 out_color;

void main(void)
{
	vec4 position = matview * vec4(in_vertex, 1.0);
	
	gl_Position = matproj * position;
	
	// transform size
	vec4 projsize = matproj * vec4(in_data.xx, position.zw);
	gl_PointSize = max(2.0, in_data.x / 64.0 * sizemult.x / projsize.z);
	
	tileID = in_data.y;
	out_normdata = in_normdata;
	out_color = in_color;
}
#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DArray texture;
uniform float daylight;

flat in float tileID;
in vec2 out_normdata;
flat in vec4 out_color;

void main(void)
{
	vec4 color = texture2DArray(texture, vec3(gl_PointCoord.x, 1.0 - gl_PointCoord.y, tileID));
	
	float alpha = out_normdata.x * color.a;
	if (alpha < 0.01) discard;
	
	#include "degamma.glsl"
	
	color.rgb = mix(color.rgb, out_color.rgb, out_color.a);
	color.rgb *= min(1.0, out_normdata[1] + daylight);
	
	// gamma ramp
	#ifdef POSTPROCESS
		gl_FragColor = vec4(color.rgb, alpha);
	#else
		gl_FragColor = vec4(pow(color.rgb, vec3(2.2)), alpha);
	#endif
}
#endif
