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
uniform sampler2D normalstex;
uniform sampler2D depthtexture;
uniform sampler2D randomtex;

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
vec3 getNormal(in vec2 uv)
{
	return normalize(texture(normalstex, uv).xyz);
}
vec3 getPosition(in vec2 uv)
{
	vec3 viewPos = vec3((uv * 2.0 - 1.0) * nearPlaneHalfSize, -1.0);
	return viewPos * linearDepth(uv);
}

const int SAMPLE_COUNT = 16;
const vec2 poisson16[] = vec2[](
	vec2( -0.94201624,  -0.39906216 ),
	vec2(  0.94558609,  -0.76890725 ),
	vec2( -0.094184101, -0.92938870 ),
	vec2(  0.34495938,   0.29387760 ),
	vec2( -0.91588581,   0.45771432 ),
	vec2( -0.81544232,  -0.87912464 ),
	vec2( -0.38277543,   0.27676845 ),
	vec2(  0.97484398,   0.75648379 ),
	vec2(  0.44323325,  -0.97511554 ),
	vec2(  0.53742981,  -0.47373420 ),
	vec2( -0.26496911,  -0.41893023 ),
	vec2(  0.79197514,   0.19090188 ),
	vec2( -0.24188840,   0.99706507 ),
	vec2( -0.81409955,   0.91437590 ),
	vec2(  0.19984126,   0.78641367 ),
	vec2(  0.14383161,  -0.14100790 ));

float getAO16(in vec3 viewPos, in float depth)
{
	float pixelSize = max(4.0, pow(1.0 - depth, 8.0) * 16.0);
	vec2 filterRadius = pixelSize / screenSize.xy;
	float distanceThreshold = 0.2 / ZFAR;
	
	// get the view space normal
	vec3 viewNormal = getNormal(texCoord);
	
    // perform AO
    float ambientOcclusion = 0.0;
	
    for (int i = 0; i < SAMPLE_COUNT; ++i)
    {
        vec2 sampleUV = texCoord + poisson16[i] * filterRadius;
        // sample at an offset specified by the current Poisson-Disk sample and scale it by a radius (has to be in Texture-Space)
		vec3 samplePos = getPosition(sampleUV);
        // distance between SURFACE-POSITION and SAMPLE-POSITION
		float VPdistSP = distance(samplePos, viewPos);
        // direction between SURFACE-POSITION and SAMPLE-POSITION
		vec3 sampleDir = (samplePos - viewPos) / VPdistSP;
		
        // angle between SURFACE-NORMAL and SAMPLE-DIRECTION (vector from SURFACE-POSITION to SAMPLE-POSITION)
        float NdotS = max(0.0, dot(viewNormal, sampleDir));
		
        // a = distance function
        float a = 1.0 - smoothstep(0.5 / ZFAR, 1.0 / ZFAR, VPdistSP / ZFAR);
		//float a = 1.0 - step(5.0, VPdistSP);
        ambientOcclusion += a * NdotS;
    }
    return ambientOcclusion / SAMPLE_COUNT;
}

void main()
{
	// base color
	color = texture2D(terrain, texCoord);
	
	// reconstruct position from window-space depth
	vec3 viewPos = eye_direction * linearDepth(texCoord);
	// viewspace/linear depth
	float depth = min(1.0, length(viewPos.xyz) / ZFAR);
	
	// Ambient Occlusion
	color.rgb *= 1.0 - getAO16(viewPos.xyz, depth);
	
	// reconstruct view to world coordinates
	vec4 wpos = vec4(viewPos, 1.0) * matview;
	// camera->point ray
	vec3 ray = normalize(-wpos.xyz);
	// to world coordinates
	wpos.xyz -= worldOffset;
	
	// volumetric fog
	float fogAmount = fogDensity(ray, wpos.xyz);
	fogAmount *= depth;
	
	const vec3 fogBaseColor = vec3(0.9);
	const vec3 sunBaseColor = vec3(1.0, 0.8, 0.5);
	
	float sunAmount = max(0.0, dot(-ray, sunAngle)) * daylight;
	vec3 fogColor = mix(fogBaseColor, sunBaseColor, sunAmount) * daylight;
	
	color.rgb = mix(color.rgb, fogColor, fogAmount);
	// additional sun glow on terrain
	color.rgb = mix(color.rgb, sunBaseColor, sunAmount * 0.5 * depth);
	
	//color.rgb = vec3(wsDepth);
	
	// mix in sky to fade out the world
	vec3 skyColor = texture(skytexture, texCoord).rgb;
	color.rgb = mix(color.rgb, skyColor, smoothstep(0.5, 1.0, depth));
	// use alpha-channel as depth
	color.a   = depth;
}

#endif
