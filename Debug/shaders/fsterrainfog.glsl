#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM
uniform vec2 nearPlaneHalfSize;

in  vec2 in_vertex;
out vec2 texCoord;
out vec3 eye_direction;

void main(void)
{
	texCoord = in_vertex;
	eye_direction = vec3((in_vertex * 2.0 - 1.0) * nearPlaneHalfSize, -1.0);
	
	gl_Position = vec4(in_vertex * 2.0 - 1.0, 0.0, 1.0);
}
#endif

#ifdef FRAGMENT_PROGRAM
uniform sampler2D terrain;
uniform sampler2D skytexture;

uniform vec3 sunAngle;

uniform mat4 matview;
uniform vec3 cameraPos;
uniform vec3 worldOffset;
uniform float timeElapsed;

in vec2 texCoord;
in vec3 eye_direction;

const float ZFAR
const float ZNEAR

#include "noise3.glsl"

float fogDensity(in vec3  ray,
				 in vec3  point,
				 in float depth,
				 in float fogheight)
{
	// distance in fog is calculated with a simple intercept theorem
	float len = max(0.0, fogheight - point.y) / max(0.1, ray.y);
	float fogdepth = min(ZFAR * 0.5, len);
	
	float noise = snoise(point * 0.01) + snoise(point * 0.04);
	
	return (noise + 3.0) * 0.2 * fogdepth / (ZFAR * 0.5);
}

void main()
{
	// base color
	vec4 color = texture2D(terrain, texCoord);
	// depth from alpha
	#define depth  color.a
	
	// add fog & sunlight
	//float sunrad = 1.0 - distance(texCoord, sunCoord) / 0.5;
	//float notSky = step(depth, 0.998);
	
	// reconstruct eye coordinates
	vec4 cofs = vec4(eye_direction * depth * ZFAR, 1.0) * matview;
	vec3 ray = normalize(-cofs.xyz);
	// to world coordinates
	vec3 wpos = cofs.xyz + vec3(0.0, cameraPos.y, 0.0) - worldOffset;
	
	// volumetric fog
	float foglevel  = fogDensity(ray, wpos, depth, 90.0);
	foglevel *= 0.1 + 0.9 * sqrt(depth);
	
	const vec3 fogBaseColor = vec3(1.0);
	const vec3 sunBaseColor = vec3(1.0, 0.8, 0.5);
	
	float sunAmount = max(0.0, dot(-ray, sunAngle)) * 0.8 * depth;
	float fogAmount = foglevel * (1.0 - sunAmount);
	//color.rgb = mix(color.rgb, fogBaseColor, fogAmount);
	color.rgb = mix(color.rgb, sunBaseColor, sunAmount);
	
	// mix in fog
	vec3 skyColor = texture2D(skytexture, texCoord).rgb;
	color.rgb = mix(color.rgb, skyColor, max(fogAmount, smoothstep(0.5, 1.0, depth)));
	
	gl_FragData[0] = color;
}

#endif
