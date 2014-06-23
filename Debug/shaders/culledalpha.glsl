#version 150
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM
uniform mat4 matmvp;
uniform mat4 matview;
uniform vec3 vtrans;

uniform float daylight;
uniform vec3  lightVector;

in vec3 in_vertex;
in vec3 in_normal;
in vec3 in_texture;
in vec4 in_biome;
in vec4 in_color;
in vec4 in_color2;

out lowp vec3 texCoord;
out lowp vec4 lightdata;
out lowp vec4 torchlight;
out lowp vec4 biomeColor;
out lowp vec3 out_normal;
out lowp vec3 v_normals;

const float VERTEX_SCALE_INV

void main(void)
{
	vec4 position = vec4(in_vertex * VERTEX_SCALE_INV + vtrans, 1.0);
	gl_Position = matmvp * position;
	
#ifdef VIEW_NORMALS
	v_normals = mat3(matview) * in_normal.xyz;
#endif
	
	texCoord = vec3(in_texture.st * VERTEX_SCALE_INV, in_texture.p);
	biomeColor = in_biome;
	lightdata  = in_color;
	torchlight = in_color2;
	out_normal = in_normal;
	
	// dotlight
	//#include "worldlight.glsl"
}
#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_explicit_attrib_location : enable

uniform sampler2DArray diffuse;
uniform sampler2DArray tonemap;

uniform float daylight;
uniform vec3  lightVector;
uniform float modulation;

in lowp vec3 texCoord;
in lowp vec4 lightdata;
in lowp vec4 torchlight;
in lowp vec4 biomeColor;
in lowp vec3 out_normal;
in lowp vec3 v_normals;

layout(location = 0) out vec4 color;
#ifdef VIEW_NORMALS
layout(location = 1) out vec4 normals;
#endif

const float ZFAR

void main(void)
{
	color = texture(diffuse, texCoord.stp);
	if (color.a < 0.1) discard;
	
	// read tonecolor from tonemap
	lowp vec4 toneColor = texture(tonemap, texCoord.stp);
	color.rgb = mix(color.rgb, biomeColor.rgb * toneColor.rgb, toneColor.a);
	
	#include "worldlight_frag.glsl"
	
	#include "degamma.glsl"
	#include "stdlight.glsl"
	#include "finalcolor.glsl"
	
#ifdef VIEW_NORMALS
	normals = vec4(v_normals, 1.0);
#endif
}

#endif
