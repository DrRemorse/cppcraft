#version 150
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM
uniform vec2 nearPlaneHalfSize;

in  vec2 in_vertex;
out vec2 texCoord;
out vec4 eye_direction;

void main(void)
{
	texCoord = in_vertex;
	gl_Position = vec4(in_vertex * 2.0 - 1.0, 0.0, 1.0);
	
	eye_direction = vec4(gl_Position.xy * nearPlaneHalfSize, -1.0, 1.0);
}
#endif

#ifdef FRAGMENT_PROGRAM
uniform sampler2D terrain;
uniform sampler2D skytexture;
uniform sampler2D depthtexture;

uniform vec3  sunAngle;
uniform float daylight;

uniform mat4 matview;
uniform vec3 cameraPos;
uniform vec3 worldOffset;
uniform float timeElapsed;

uniform mat4 matviewport;
uniform mat4 matmvp;
uniform mat4 matproj;

in vec2 texCoord;
in vec4 eye_direction;

const float ZFAR
const float ZNEAR

#include "noise3.glsl"

float fogDensity(in vec3  ray,
				 in vec3  point,
				 in float depth)
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
	
	point.x += timeElapsed * 0.02;
	float noise = snoise(point * 0.02) + snoise(point * 0.05); // + snoise(point * 2.0);
	noise = (noise + 2.0) * 0.25;
	
	return (noise + foglen) * 0.5 * foglevel;
}

float linearizeDepth(in vec2 uv)
{
	float d = texture2D(depthtexture, uv).x;
	return ZNEAR / (ZFAR - d * (ZFAR - ZNEAR)) * ZFAR;
}

void main()
{
	// base color
	vec4 color = texture2D(terrain, texCoord);
	// depth from alpha
	#define depth  color.a
	
	// reconstruct eye coordinates
	vec4 cofs = eye_direction * matview;
	cofs.xyz *= linearizeDepth(texCoord);
	vec3 ray = normalize(-cofs.xyz);
	// to world coordinates
	vec3 wpos = cofs.xyz + vec3(0.0, cameraPos.y, 0.0) - worldOffset;
	
	// volumetric fog
	float fogAmount = fogDensity(ray, wpos, depth);
	fogAmount *= depth;
	
	vec3 fogBaseColor = vec3(0.9) * daylight;
	vec3 sunBaseColor = vec3(1.0, 0.8, 0.5) * daylight;
	
	float sunAmount = max(0.0, dot(-ray, sunAngle));
	vec3 fogColor = mix(fogBaseColor, sunBaseColor, sunAmount);
	
	color.rgb = mix(color.rgb, fogColor, fogAmount);
	// additional sun glow on terrain
	color.rgb = mix(color.rgb, sunBaseColor, sunAmount * 0.5 * depth);
	
	//color.rgb = wpos.xyz / ZFAR;
	//color.rgb = vec3(fogAmount*4.0);
	
	// mix in fog
	vec3 skyColor = texture2D(skytexture, texCoord).rgb;
	color.rgb = mix(color.rgb, skyColor, smoothstep(0.5, 1.0, depth));
	
	gl_FragData[0] = color;
}

#endif
