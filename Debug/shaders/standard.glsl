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
in vec4 in_texture;
in vec4 in_biome;
in vec4 in_color;
in vec4 in_color2;

out vec3 texCoord;
out vec4 biomeColor;
out vec4 lightdata;
out vec4 torchlight;
flat out float worldLight;

out float vertdist;
//flat out vec3 out_normal;

//flat out float reflection;
//out vec3 v_reflect;

const int TX_REPEAT
const int TX_SOLID

const float VERTEX_SCALE
const float ICE_TILE = 13.f * 16.f + 14.f;

void main(void)
{
	vec4 position = vec4(in_vertex / VERTEX_SCALE + vtrans, 1.0);
	position = matview * position;
	vertdist = length(position.xyz);
	gl_Position = matproj * position;
	
	/* ice reflection */
	/*reflection = 0.0;
	v_eye = -position.xyz / vertdist;
	if (in_texture.p == ICE_TILE)
	{
		reflection = 1.0;
		
		// real reflection vector
		v_reflect = reflect((-v_eye) * mat3(matview), in_normal);
	}*/
	
	texCoord = vec3(in_texture.st / VERTEX_SCALE, in_texture.p);
	
	//out_normal = in_normal.xyz * 2.0 - vec3(1.0);
	
	// dotlight
	#include "worldlight.glsl"
	
	biomeColor = in_biome;
	lightdata  = in_color;
	torchlight = in_color2;
}

#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DArray texture;
uniform sampler2DArray tonemap;
//uniform samplerCube skymap;

uniform vec3 screendata;

uniform float daylight;
uniform float modulation;
uniform vec4  playerLight;

in vec3 texCoord;
in vec4 biomeColor;
in vec4 lightdata;
in vec4 torchlight;
flat in float worldLight;

in float vertdist;
//flat in vec3 out_normal;

flat in float reflection;
in vec3 v_reflect;

const float ZFAR
const int TX_SOLID
const int TX_CROSS

void main(void)
{
	// independent texture reads using inbound variable directly
	// read tonecolor from tonemap
	vec4 color = texture2DArray(tonemap, texCoord);
	color.rgb *= biomeColor.rgb;
	
	// mix diffuse map
	vec4 diffuse = texture2DArray(texture, texCoord);
	color = mix(diffuse, color, color.a);
	
	// reflection //
	/*if (reflection > 0.15)
	{
		float fresnel = 1.0 - v_reflect.y * v_reflect.y;
		
		// mix in reflection
		vec3 reflection = mix(textureCube(skymap, v_reflect).rgb, vec3(0.75), 0.75);
		color.rgb = mix(color.rgb, reflection, fresnel * 0.5);
	}*/
	
	#include "degamma.glsl"
	
	#include "stdlight.glsl"
	
	#include "horizonfade.glsl"
	
	#include "finalcolor.glsl"
}
#endif
