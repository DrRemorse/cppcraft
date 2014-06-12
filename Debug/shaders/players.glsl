#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM
uniform mat4 matproj;
uniform mat4 matview;
uniform mat4 matrot;

uniform vec3 lightVector;
uniform float modulation;
uniform float skinmodel;

in vec3 in_vertex;
in vec3 in_normal;
in vec4 in_texture;

out vec3 texCoord;

out float vertdist;
flat out float worldLight;
out float brightness;

const float TEX_SCALE = 16.0;
const float ZFAR

void main()
{
	vec4 position = vec4(in_vertex.xyz, 1.0);
	position = matview * position;
	vertdist = length(position);
	gl_Position = matproj * position;
	
	texCoord = in_texture.stp;
	texCoord.st /= TEX_SCALE;
	texCoord.p += skinmodel * 16.0;
	
	// view normals
	vec3 normal = mat3(matrot) * in_normal.xyz;
	
	// worldlight //
	const float ambience = 0.5;
	float dotlight = dot(normal, lightVector.xyz);
	worldLight = max(ambience * 0.6 + 0.5 * (0.5 + 0.5*dotlight), 0.2);
	// worldlight //
}

#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable
uniform sampler2DArray texture;
uniform float daylight;

in vec3 texCoord;

in float vertdist;
flat in float worldLight;
in float brightness;

out vec4 color;

void main(void)
{
	color = texture2DArray(texture, texCoord);
	if (color.a < 0.1) discard; // le discard!
	
	#include "degamma.glsl"
	
	// shadows & torchlight
	color.rgb *= worldLight * daylight;
	
	#include "finalcolor.glsl"
}

#endif
