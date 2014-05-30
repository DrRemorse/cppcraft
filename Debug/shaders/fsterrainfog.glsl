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

float ambientOcclusion(in vec2 coord, in vec3 pos, in vec3 normal)
{
	const float g_scale = 0.1;
	const float g_bias  = 0.0;
	const float g_intensity = 1.2;
	
	vec3 samplePos = getPosition(coord);
	vec3 diff = samplePos - pos;
	
	float d = length(diff);
	vec3  v = diff / d;
	
	vec3 sampleNorm = getNormal(coord);
	float dotp = min(1.0, 1.0 - dot(normal, sampleNorm));
	dotp = dotp * dot(normal, v);
	
	d = 1.0 / (1.0 + d * g_scale);
	return max(0.0, dotp - g_bias) * d * g_intensity;
}

float getAO(vec2 uv)
{
	vec3 p = getPosition(uv);
	vec3 n = getNormal(uv);
	vec2 rand = texture(randomtex, texCoord * 8.0).rg * 1.0 - 0.5;
	
	const float sample_radius = 0.2;
	const int iterations = 4;
	
	const vec2 offsetsAO[4] = vec2[]
	(
		vec2(1.0, 0.0), vec2(-1.0, 0.0),
		vec2(0.0, 1.0), vec2( 0.0,-1.0)
	);
	
	float ao  = 0.0;
	float rad = sample_radius / p.z;
	
	//** SSAO Calculation **//
	for (int j = 0; j < iterations; ++j)
	{
		vec2 coord1 = reflect(offsetsAO[j], rand) * rad;
		vec2 coord2 = vec2(coord1.x*0.707 - coord1.y*0.707,
						   coord1.x*0.707 + coord1.y*0.707);
	  
	  ao += ambientOcclusion(uv + coord1*0.25, p, n);
	  ao += ambientOcclusion(uv + coord2*0.50, p, n);
	  ao += ambientOcclusion(uv + coord1*0.75, p, n);
	  ao += ambientOcclusion(uv + coord2, p, n);
	}
	return ao / (float(iterations) * 4.0);
}

void main()
{
	// base color
	color = texture2D(terrain, texCoord);
	// depth from alpha
	#define depth  color.a
	
	// reconstruct view to world coordinates
	vec4 cofs = eye_direction * matview;
	cofs.xyz *= linearizeDepth(texCoord);
	// camera->point ray
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
	vec3 normal = texture(normalstex, texCoord).xyz;
	//color.rgb = normal;
	color.rgb *= 1.0 - getAO(texCoord);
	
	// mix in sky to fade out the world
	vec3 skyColor = texture2D(skytexture, texCoord).rgb;
	color.rgb = mix(color.rgb, skyColor, smoothstep(0.5, 1.0, depth));
}

#endif
