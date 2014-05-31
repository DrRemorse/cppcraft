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
uniform sampler2D normalstex;
uniform sampler2D depthtexture;
uniform sampler2D randomtex;

uniform vec3  sunAngle;
uniform float daylight;

uniform mat4 matview;
uniform vec3 cameraPos;
uniform vec3 worldOffset;
uniform float timeElapsed;
uniform vec2 nearPlaneHalfSize;

in  vec2 texCoord;
in  vec4 eye_direction;
out vec4 color;

const float ZFAR
const float ZNEAR

//#include "noise3.glsl"

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
	
	point.x += timeElapsed * 0.02;
	float noise = 0.0; //snoise(point * 0.02) + snoise(point * 0.05); // + snoise(point * 2.0);
	noise = (noise + 2.0) * 0.25;
	
	return (noise + foglen) * 0.5 * foglevel;
}

float linearizeDepth(in vec2 uv)
{
	float d = texture2D(depthtexture, uv).x;
	return ZNEAR / (ZFAR - d * (ZFAR - ZNEAR)) * ZFAR;
}
vec3 getNormal(in vec2 uv)
{
	return texture(normalstex, uv).xyz;
}
vec3 getPosition(in vec2 uv)
{
	vec3 cofs = vec3(uv * nearPlaneHalfSize, -1.0);
	return cofs * linearizeDepth(uv);
}

const   float distanceThreshold = 0.5;
uniform vec2  filterRadius;

const int sample_count = 16;
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

float getAO16(in vec3 viewPos)
{
	// get the view space normal
	vec3 viewNormal = getNormal(texCoord);
	
    float ambientOcclusion = 0.0;
    // perform AO
    for (int i = 0; i < sample_count; ++i)
    {
        // sample at an offset specified by the current Poisson-Disk sample and scale it by a radius (has to be in Texture-Space)
        vec3 samplePos = getPosition(texCoord + poisson16[i] * filterRadius);
        // distance between SURFACE-POSITION and SAMPLE-POSITION
		float VPdistSP = distance(samplePos, viewPos);
        // direction between SURFACE-POSITION and SAMPLE-POSITION
		vec3 sampleDir = (samplePos - viewPos) / VPdistSP;
		
        // angle between SURFACE-NORMAL and SAMPLE-DIRECTION (vector from SURFACE-POSITION to SAMPLE-POSITION)
        float NdotS = max(0.0, dot(viewNormal, sampleDir));
		
        // a = distance function
        float a = 1.0 - smoothstep(distanceThreshold, distanceThreshold * 2, VPdistSP / ZFAR);
		
        ambientOcclusion += a * NdotS;
    }
    return ambientOcclusion / sample_count;
}

void main()
{
	// base color
	color = texture2D(terrain, texCoord);
	// depth from alpha
	#define depth  color.a
	
	// reconstruct position from depth
	vec3 viewPos = getPosition(texCoord);
	
	// Ambient Occlusion
	color.rgb *= 1.0 - getAO16(viewPos) * 0.7;
	
	// reconstruct view to world coordinates
	vec4 cofs = vec4(viewPos, 1.0) * matview;
	// camera->point ray
	vec3 ray = normalize(-cofs.xyz);
	// to world coordinates
	vec3 wpos = cofs.xyz + vec3(0.0, cameraPos.y, 0.0) - worldOffset;
	
	// volumetric fog
	float fogAmount = fogDensity(ray, wpos);
	fogAmount *= depth;
	
	vec3 fogBaseColor = vec3(0.9) * daylight;
	vec3 sunBaseColor = vec3(1.0, 0.8, 0.5) * daylight;
	
	float sunAmount = max(0.0, dot(-ray, sunAngle));
	vec3 fogColor = mix(fogBaseColor, sunBaseColor, sunAmount);
	
	color.rgb = mix(color.rgb, fogColor, fogAmount);
	// additional sun glow on terrain
	color.rgb = mix(color.rgb, sunBaseColor, sunAmount * 0.5 * depth);
	
	//color.rgb = wpos.xyz / ZFAR;
	
	// mix in sky to fade out the world
	vec3 skyColor = texture2D(skytexture, texCoord).rgb;
	color.rgb = mix(color.rgb, skyColor, smoothstep(0.5, 1.0, depth));
}

#endif
