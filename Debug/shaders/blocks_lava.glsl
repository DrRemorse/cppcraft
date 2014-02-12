#version 330
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM
//precision mediump float;

uniform mat4 matproj;
uniform mat4 matview;
uniform vec3 vtrans;

uniform vec3 worldOffset;

in vec3 in_vertex;
in vec3 in_normal;
in vec3 in_texture;
in vec4 in_biome;
in vec4 in_color;
in vec4 in_color2;

out float vertdist;
out vec2  wave;

const float VERTEX_SCALE
const float ZFAR

void main(void)
{
	vec4 position = vec4(in_vertex / VERTEX_SCALE + vtrans, 1.0);
	position = matview * position;
	vertdist = length(position.xyz) / ZFAR;
	gl_Position = matproj * position;
	
	// transformed wave coordinates
	wave = (position.xyz * mat3(matview) - worldOffset).xz;
}

#endif

#ifdef FRAGMENT_PROGRAM

uniform sampler2D texture;
uniform vec3 screendata;

uniform float frameCounter;

in float vertdist;
in vec2  wave;

#include "noise.glsl"

void main(void)
{
	float timer = frameCounter / 800.0;
	
	vec2 wave1 = wave * 1.0 + timer * vec2(1, 0) + sin(timer * vec2(1));
	vec2 wave2 = wave * 2.0 + timer * vec2(0, 1);
	
	// animated lava
	float n = snoise(wave1) * snoise(wave2);
	n = smoothstep(0.2, 0.6, n * 0.5 + 0.5);
	
	const vec3 lavaHigh = vec3(1.0, 0.8, 0.3);
	const vec3 lavaLow  = vec3(0.5, 0.0, 0.1);
	
	vec3 lavaColor = mix(lavaHigh, lavaLow, n);
	
	// under the lava
	vec2 texCoord =  gl_FragCoord.xy / screendata.xy;
	vec4 underlava = texture2D(texture, texCoord);
	
	float wdepth = 0.02 + max(0.0, underlava.a - vertdist);
	
	float dep = smoothstep(0.0, 0.04, wdepth);
	
	// mix it all together
	vec3 color = mix(underlava.rgb, lavaColor, dep);
	
	gl_FragData[0] = vec4(color, vertdist);
}

#endif
