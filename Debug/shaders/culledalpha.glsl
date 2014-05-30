#version 150
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM
uniform mat4 matproj;
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

out vec3 texCoord;
out vec4 lightdata;
out vec4 torchlight;
out vec4 biomeColor;
flat out float worldLight;

out float vertdist;

const float VERTEX_SCALE_INV

void main(void)
{
	vec4 position = vec4(in_vertex * VERTEX_SCALE_INV + vtrans, 1.0);
	position = matview * position;
	vertdist = length(position.xyz);
	gl_Position = matproj * position;
	
	texCoord = vec3(in_texture.st * VERTEX_SCALE_INV, in_texture.p);
	
	// dotlight
	#include "worldlight.glsl"
	
	biomeColor = in_biome;
	lightdata  = in_color;
	torchlight = in_color2;
}
#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DArray diffuse;
uniform sampler2DArray tonemap;

uniform float daylight;
uniform vec4  playerLight;
uniform float modulation;

in vec3 texCoord;
in vec4 lightdata;
in vec4 torchlight;
in vec4 biomeColor;
flat in float worldLight;

in float vertdist;
const float ZFAR

out vec4 color;

void main(void)
{
	color = texture(diffuse, texCoord.stp);
	if (color.a < 0.02) discard;
	
	// read tonecolor from tonemap
	vec4 toneColor = texture(tonemap, texCoord.stp);
	color.rgb = mix(color.rgb, biomeColor.rgb * toneColor.rgb, toneColor.a);
	
	#include "degamma.glsl"
	
	#include "stdlight.glsl"
	
	#include "horizonfade.glsl"
	
	#include "finalcolor.glsl"
}

#endif
