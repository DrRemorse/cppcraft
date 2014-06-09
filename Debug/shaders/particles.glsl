#version 150
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#define POSTPROCESS

#ifdef VERTEX_PROGRAM
uniform mat4 matproj;
uniform mat4 matview;

uniform vec2 screensize;
uniform float fovRadians;

in vec3 in_vertex;
in vec2 in_data;
in vec2 in_normdata;
in vec4 in_color;

flat out float tileID;
flat out vec2 out_normdata;
flat out vec4 out_color;

void main(void)
{
	vec4 position = matview * vec4(in_vertex, 1.0);
	gl_Position = matproj * position;
	
	// transform size
	vec4 projsize = matproj * vec4(in_data.xx, position.zw);
	gl_PointSize = max(2.0, in_data.x / 64.0 * screensize.x / projsize.z);
	
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
flat in vec2 out_normdata;
flat in vec4 out_color;

out vec4 color;

void main(void)
{
	color = texture2DArray(texture, vec3(gl_PointCoord.xy, tileID));
	
	#include "degamma.glsl"
	
	color.rgb = mix(color.rgb, out_color.rgb, out_color.a);
	color.rgb *= min(1.0, out_normdata.y + daylight);
	color.a *= out_normdata.x;
	
	#include "finalcolor.glsl"
}
#endif
