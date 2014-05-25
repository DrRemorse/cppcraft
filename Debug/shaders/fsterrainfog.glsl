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

uniform float sundot;
uniform vec2 sunCoord;

uniform mat4 matview;
uniform vec3 cameraPos;
uniform vec3 worldOffset;
uniform float timeElapsed;

in vec2 texCoord;
in vec3 eye_direction;

const float ZFAR
const float ZNEAR

#include "noise.glsl"

float fogDensity(in vec3  camera,
                 in vec3  point,
				 in float fogHeight)
{
	vec3 rayDir = normalize(camera - point);
	rayDir.y = max(rayDir.y, 0.1);
	
	// distance in fog is calculated with a simple intercept theorem
	float L = max(0.0, (fogHeight - point.y) / rayDir.y);
	
	const float growth = 0.3;
	const float density = 0.3;
	
	// when dist is 0, log(dist) is 1, so subtract this
	float fogAmount = (log(L * growth) - 1) * density;
	return max(0.0, fogAmount);
}

void main()
{
	// base color
	vec4 color = texture2D(terrain, texCoord);
	// depth from alpha
	float depth = color.a;
	
	// add fog & sunlight
	float sunrad = 1.0 - distance(texCoord, sunCoord) / 0.5;
	float notSky = step(depth, 0.998);
	
	// volumetric fog
	vec4 cofs = vec4(eye_direction * depth * ZFAR, 1.0) * matview;
	vec3 tpos = cameraPos + cofs.xyz;
	
	vec3 noisePos = tpos - cameraPos - worldOffset;
	noisePos.x += timeElapsed * 0.05;
	
	float noise = snoise(noisePos.xz * 0.02);
	
	// density calculation
	float foglevel = fogDensity(cameraPos, tpos, 80.0 + 10 * noise);
	foglevel = (foglevel * 0.5 + 0.5) * depth;
	
	const vec3 fogBaseColor = vec3(0.9);
	const vec3 sunBaseColor = vec3(1.0, 0.8, 0.5);
	
	float sunAmount = max(0.0, sunrad) * notSky * sundot * 0.9 * depth;
	float fogAmount = foglevel;
	color.rgb = mix(color.rgb, fogBaseColor, fogAmount * (1.0 - sunAmount));
	color.rgb = mix(color.rgb, sunBaseColor, sunAmount);
	
	// mix in fog
	vec3 skyColor = texture2D(skytexture, texCoord).rgb;
	color.rgb = mix(color.rgb, skyColor, smoothstep(0.5, 1.0, depth));
	
	gl_FragData[0] = color;
}

#endif
