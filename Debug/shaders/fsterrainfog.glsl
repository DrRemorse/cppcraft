#version 150
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
	gl_Position = vec4(in_vertex * 2.0 - 1.0, 0.0, 1.0);
	
	eye_direction = vec3(gl_Position.xy * nearPlaneHalfSize, -1.0);
}
#endif

#ifdef FRAGMENT_PROGRAM
uniform sampler2D terrain;
uniform sampler2D skytexture;
uniform sampler2D depthtexture;
#ifdef AMBIENT_OCCLUSION
uniform sampler2D normalstex;
#endif

uniform vec3  sunAngle;
uniform float daylight;

uniform mat4 matview;
uniform vec3 worldOffset;
uniform float timeElapsed;
uniform vec2 nearPlaneHalfSize;
uniform vec2 screenSize;

in  vec2 texCoord;
in  vec3 eye_direction;
out vec4 color;

const float ZFAR
const float ZNEAR

#include "noise3.glsl"

float fogDensity(in vec3  ray,
				 in vec3  point)
{
	const float HEIGHT   = 36.0;
	const float fogY     = 70.0;
	const float fogTopY  = fogY + HEIGHT;
	
	// distance in fog is calculated with a simple intercept
	float foglen = max(0.0, fogTopY - point.y) / abs(ray.y);
	foglen = min(1.0, foglen / ZFAR);
	
	// how far are we from center of fog?
	float foglevel = min(1.0, abs(point.y - fogY) / HEIGHT);
	foglevel = 1.0 - foglevel * foglevel;
	
	vec3 np1 = point + vec3(timeElapsed * 0.02, 0.0, 0.0);
	vec3 np2 = point + vec3(timeElapsed * 0.03, 0.0, 0.0);
	float noise = snoise(np1 * 0.02) + snoise(np2 * 0.05) + 2.0;
	
	return (noise * 0.25 + foglen) * 0.5 * foglevel;
}

float linearDepth(in vec2 uv)
{
	float wsDepth = texture(depthtexture, uv).x;
	return ZNEAR / (ZFAR - wsDepth * (ZFAR - ZNEAR)) * ZFAR;
}
vec3 getPosition(in vec2 uv)
{
	vec3 viewPos = vec3((uv * 2.0 - 1.0) * nearPlaneHalfSize, -1.0);
	return viewPos * linearDepth(uv);
}

#ifdef AMBIENT_OCCLUSION
	#include "ssao.glsl"
#endif

void main()
{
	// base color
	color = texture2D(terrain, texCoord);
	
	// reconstruct position from window-space depth
	vec3 viewPos = eye_direction * linearDepth(texCoord);
	// viewspace/linear depth
	float depth = min(1.0, length(viewPos.xyz) / ZFAR);
	
#ifdef AMBIENT_OCCLUSION
	// Ambient Occlusion
	color.rgb *= 1.0 - getAO16(viewPos.xyz, depth);
#endif
	
	// reconstruct view to world coordinates
	vec4 wpos = vec4(viewPos, 1.0) * matview;
	// camera->point ray
	vec3 ray = normalize(-wpos.xyz);
	// to world coordinates
	wpos.xyz -= worldOffset;
	
	// volumetric fog
	float fogAmount = fogDensity(ray, wpos.xyz) * 0.5;
	fogAmount *= max(0.0, depth - 0.2) * daylight;
	
	const vec3 fogBaseColor = vec3(0.9);
	const vec3 sunBaseColor = vec3(1.0, 0.8, 0.5);
	
	float sunAmount = max(0.0, dot(-ray, sunAngle)) * daylight * daylight;
	vec3 fogColor = mix(fogBaseColor, sunBaseColor, sunAmount);
	
	color.rgb = mix(color.rgb, fogColor, fogAmount);
	// additional sun glow on terrain
	color.rgb = mix(color.rgb, sunBaseColor, sunAmount * 0.5 * depth);
	
	//color.rgb = vec3(depth);
	
	// mix in sky to fade out the world
	vec3 skyColor = texture(skytexture, texCoord).rgb;
	const float SKY_EDGE = 0.7;
	float edge = max(0.0, (depth - SKY_EDGE) / (1.0 - SKY_EDGE));
	color.rgb = mix(color.rgb, skyColor, edge * edge);
	// use alpha-channel as depth
	color.a   = depth;
}

#endif
