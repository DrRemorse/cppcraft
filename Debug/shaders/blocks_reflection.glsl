#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM
precision mediump float;

#ifdef VERTEX_PROGRAM

uniform mat4 matproj;
uniform mat4 matview;
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

out float vertdist;
const float VERTEX_SCALE

void main(void)
{
	vec4 position = vec4(in_vertex / VERTEX_SCALE + vtrans, 1.0);
	position = matview * position;
	vertdist = length(position.xyz);
	gl_Position = matproj * position;
	
	texCoord = vec3(in_texture.st / VERTEX_SCALE, in_texture.p);
	
	// dotlight
	#include "worldlight.glsl"
	
	lightdata  = in_color;
	torchlight = in_color2;
	biomeColor = in_biome;
}

#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DArray texture;
uniform sampler2DArray tonemap;

uniform float daylight;
uniform float modulation;
//uniform vec4  playerLight;

in vec3 texCoord;
in vec4 lightdata;
in vec4 torchlight;
in vec4 biomeColor;
flat in float worldLight;

in float vertdist;

const float ZFAR

void main(void)
{
	// independent texture reads using inbound variable directly
	// read tonecolor from tonemap
	vec4 color = texture2DArray(tonemap, texCoord);
	color.rgb *= biomeColor.rgb;
	
	// mix diffuse map
	vec4 diffuse = texture2DArray(texture, texCoord);
	color = mix(diffuse, color, color.a);
	
	#include "degamma.glsl"
	
	#include "stdlight.glsl"
	
	#include "horizonfade.glsl"
	
	// fake fog
	const vec3 fogColor = vec3(0.7);
	color.rgb = mix(color.rgb, fogColor, vertdist / ZFAR * 0.25);
	
	// gamma again
	gl_FragData[0] = vec4(pow(color.rgb, vec3(2.2)), color.a);
}
#endif

