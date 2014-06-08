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

out vec3 texCoord;
out vec4 lightdata;
out vec4 torchlight;
out vec4 biomeColor;
flat out float worldLight;

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

in vec3 texCoord;
in vec4 lightdata;
in vec4 torchlight;
in vec4 biomeColor;
flat in float worldLight;

const float ZFAR

out vec4 color;

void main(void)
{
	// independent texture reads using inbound variable directly
	// read tonecolor from tonemap
	vec4 tone = texture(tonemap, texCoord);
	tone.rgb *= biomeColor.rgb;
	
	// mix diffuse map
	color = texture(diffuse, texCoord);
	color.rgb = mix(color.rgb, tone.rgb, tone.a);
	
	#include "degamma.glsl"
	
	#include "stdlight.glsl"
	
	// fake fog
	vec3 fogColor = vec3(0.6, 0.7, 0.8) * daylight;
	color.rgb = mix(color.rgb, fogColor, color.a * 0.75);
}
#endif
