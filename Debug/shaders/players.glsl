#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM
uniform mat4 matproj;
uniform mat4 matview;
uniform vec3 vtrans;

uniform vec4 lightVector;
uniform vec4 playerLight;
uniform float modulation;

in vec3 in_vertex;
in vec3 in_normal;
in vec4 in_texture;
in vec4 in_biome;
in vec4 in_color;
in vec4 in_color2;

out vec3 texCoord;
out vec4 colordata;

out float vertdist;
flat out float worldLight;
out float brightness;

const float VERTEX_SCALE
const float ZFAR

void main()
{
	vec4 position = vec4(in_vertex.xyz / VERTEX_SCALE + vtrans, 1.0);
	position = gl_ModelViewMatrix * position;
	vertdist = length(position);
	gl_Position = gl_ProjectionMatrix * position;
	
	texCoord = in_texture.stp;
	texCoord.st /= VERTEX_SCALE;
	
	/* worldlight */
	const float ambience = 0.5;
	float dotlight = dot(in_normal.xyz, lightVector.xyz);
	worldLight = max(ambience * 0.6 + 0.5 * (0.5 + 0.5*dotlight), 0.2);
	/* worldlight */
	
	colordata = in_color;
}

#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DArray texture;
uniform float daylight;

in vec3 texCoord;
in vec4 colordata;

in float vertdist;
flat in float worldLight;
in float brightness;


void main(void)
{
	vec4 color = texture2DArray(texture, texCoord);
	if (color.a < 0.1) discard; // le discard!
	
	#include "degamma.glsl"
	
	// shadows & torchlight
	color.rgb *= worldLight * min(1.0, daylight + brightness);
	//color.rgb = mix(color.rgb, colordata.rgb, max(0.0, colordata.a - brightness));
	
	#include "finalcolor.glsl"
}

#endif
