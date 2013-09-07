#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM
precision mediump float;

#ifdef VERTEX_PROGRAM
uniform mat4 matproj;
uniform mat4 matview;
uniform vec3 vtrans;

uniform float frameCounter;
uniform vec3 lightVector;
uniform int  texrange;

in vec3 in_vertex;
in vec3 in_normal;
in vec3 in_tangent;
in vec3 in_texture;
in vec4 in_color;
in vec4 in_color2;
in vec4 in_biome;

out vec3 texCoord;
out vec4 lightdata;
out vec4 torchlight;
out vec4 biomeColor;
out vec3 out_normal;

out float vertdist;

const int TX_2SIDED
const int TX_CROSS

const float ZFAR
const float VERTEX_SCALE
const float CROSSWIND_STRENGTH  = 0.125;
const float PI2                 = 6.28318530717;

void main(void)
{
	vec4 position = vec4(in_vertex / VERTEX_SCALE + vtrans, 1.0);
	position = matview * position;
	vertdist = length(position.xyz);
	
	texCoord = vec3(in_texture.st / VERTEX_SCALE, in_texture.p);
	
	if (texrange == TX_CROSS && vertdist < ZFAR * 0.2)
	{
		// fire animation
		if (texCoord.z == 224.0)
		{
			texCoord.z += mod(int(frameCounter / PI2 * 32.0 + length(in_vertex.xz) / 2.0), 8.0);
		}
		
		// standing
		float speed  = frameCounter * 0.01;
		// crosses waving in the wind
		vec2 pos = in_vertex.xz / (VERTEX_SCALE * 16.0);
		if (texCoord.z == 94.0)
		{
			// hanging
			position.x += (sin(PI2 * (2.0*pos.x + pos.y) + speed) + 0.6) * CROSSWIND_STRENGTH * (1.0 - texCoord.t);
		}
		else
		{
			//             pow(min(1.0, vertdist / 2.0), 3.0) * 
			float factor = CROSSWIND_STRENGTH * texCoord.t;
			
			position.x += sin(PI2 * (2.0*pos.x + pos.y) + speed) * factor;
			
		} // hanging & standing
	}
	gl_Position = matproj * position;
	
	lightdata  = in_color;
	torchlight = in_color2;
	biomeColor = in_biome;
	out_normal = in_normal;
}
#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DArray texture;
uniform sampler2DArray tonemap;

uniform vec3 screendata;
uniform vec3 lightVector;
uniform float daylight;
uniform float modulation;

uniform vec4  playerLight;
uniform int   texrange;

in vec3 texCoord;
in vec4 lightdata;
in vec4 torchlight;
in vec4 biomeColor;
in vec3 out_normal;

in float vertdist;

const float ZFAR
const int TX_2SIDED
const int TX_CROSS

void main(void)
{
	#define coord texCoord
	
	vec4 color = texture2DArray(texture, coord.stp);
	if (color.a < 0.1 || vertdist >= ZFAR) discard;
	
	// read tonecolor from tonemap
	vec4 toneColor = texture2DArray(tonemap, coord.stp);
	color.rgb = mix(color.rgb, biomeColor.rgb * toneColor.rgb, toneColor.a);
	
	#include "degamma.glsl"
	
	#include "worldlight_frag.glsl"
	
	#include "stdlight.glsl"
	
	#include "horizonfade.glsl"
	
	#include "finalcolor.glsl"
}

#endif
