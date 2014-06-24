#version 150
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM
uniform mat4 matmvp;
uniform vec3 vtrans;

uniform vec3  lightVector;
uniform float daylight;

in vec3 in_vertex;
in vec3 in_normal;
in vec3 in_texture;
in vec4 in_color;
in vec4 in_color2;
in vec4 in_biome;

out lowp vec3 texCoord;
out lowp vec4 lightdata;
out lowp vec4 torchlight;
out lowp vec4 biomeColor;
flat out lowp float worldLight;

const float VERTEX_SCALE_INV

void main(void)
{
	vec4 position = vec4(in_vertex * VERTEX_SCALE_INV + vtrans, 1.0);
	gl_Position = matmvp * position;
	
	texCoord = vec3(in_texture.st * VERTEX_SCALE_INV, in_texture.p);
	
	// dotlight
	#include "worldlight.glsl"
	
	lightdata  = in_color;
	torchlight = in_color2;
	biomeColor = in_biome;
}

#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DArray diffuse;
uniform sampler2DArray tonemap;

uniform float daylight;
uniform float modulation;

in lowp vec3 texCoord;
in lowp vec4 lightdata;
in lowp vec4 torchlight;
in lowp vec4 biomeColor;
flat in lowp float worldLight;

const float ZFAR

out lowp vec4 color;

void main(void)
{
	// independent texture reads using inbound variable directly
	// read tonecolor from tonemap
	lowp vec4 tone = texture(tonemap, texCoord);
	tone.rgb *= biomeColor.rgb;
	
	// mix diffuse map
	color = texture(diffuse, texCoord);
	color.rgb = mix(color.rgb, tone.rgb, tone.a);
	
	#include "degamma.glsl"
	
	#include "stdlight.glsl"
	
	// fake fog
	lowp vec3 fogColor = vec3(1.0) * daylight;
	color.rgb = mix(color.rgb, fogColor, 0.1);
}
#endif
