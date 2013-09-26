#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#define NOISE_CLOUDS
#define POSTPROCESS

#ifdef VERTEX_PROGRAM
uniform mat4 matproj;
uniform mat4 matview;

uniform float frameCounter;
uniform vec3  worldOffset;
uniform vec3  lightVector;

in vec3 in_vertex;

out vec2 texCoord;
out float vertdist;

out vec3 v_ldir;
out vec3 v_eye;

const float ZFAR
const float planetoid = 32.0;
const float skyslope  =  2.0;

void main()
{
	vec4 position = matview * vec4(in_vertex, 1.0);
	
	mat4 matrot = mat4(mat3(matview));
	vec4 lpos = position * matrot;
	vertdist = length(lpos.xz);
	
	// reposition, by adding planetoidal radius
	lpos.y -= pow(vertdist / ZFAR, skyslope) * planetoid;
	// new vertex distance
	vertdist = length(lpos.xyz);
	// new position
	position = matrot * lpos;
	
	v_ldir = mat3(matview) * lightVector;
	v_eye = normalize(-position.xyz);
	
	#ifdef NOISE_CLOUDS
		// size minification
		texCoord = (lpos.xz - worldOffset.xz) / 32.0 / 6.0;
	#else
		texCoord = (lpos.xz - worldOffset.xz) / 48.0 / 8.0;
	#endif
	// movement speed
	texCoord.s += frameCounter * 0.0001;
	
	gl_Position = matproj * position;
}

#endif

#ifdef FRAGMENT_PROGRAM
uniform sampler2D texture;

uniform float frameCounter;
uniform float daylight; // multiplier

uniform mat4 matview;

in vec2 texCoord;
in float vertdist;

in vec3 v_ldir;
in vec3 v_eye;

const float ZFAR

#include "noise.glsl"

float nmix2(float x)
{
	return x * 0.5 + (0.5-abs(x)) * (0.5*2);
}

void main(void)
{
	const float maxdist  = ZFAR * 1.6;
	if (vertdist >= maxdist) discard;
	
#ifdef NOISE_CLOUDS
	#define p texCoord.st
	
	vec2 w = vec2(frameCounter * 0.00005, 0);
	
	float alp = 2.0*abs(snoise(p+ w*1.4)) - 1.0   // dependent reads
				+ abs(snoise(p * 2.0 + w))* 0.75
				+ abs(snoise(p * 4.0 - w*0.7))* 0.37
				+ nmix2(snoise(p * 8.0 - w*0.5))* 0.125 * 1.25
				+ nmix2(snoise(p * 16.0 + w*0.3))* 0.0625 * 1.25;
	
	float edge = 2.5;
	float dark = 0.17 / edge;
	float density = -0.3;
	
	alp += density;
	alp *= edge;
	dark*= alp;
	
	alp = clamp(alp, 0.0, 1.0);
	alp = alp*alp*(3 - 2*alp);
	alp *= alp;
	alp *= 0.8 * 2.0;
	
	if (alp < 0.05) discard;
	
	vec4 color = vec4(1.05 - dark*1.22, 1.05 - dark*1.2, 1.05 - dark, alp);
	
#else
	// textured clouds
	
	vec4 noise = texture2D(texture, texCoord); // independent read
	
	vec3 normal = noise.rgb * 2.0 - vec3(1.0);
	
	const vec3 in_normal  = vec3( 0.0,  1.0,  0.0);
	const vec3 in_tangent = vec3( 1.0,  0.0,  0.0);
	const vec3 binormal   = vec3( 0.0,  0.0, -1.0);
	
	const mat3 tbn = mat3(in_tangent, binormal, in_normal);
	
	normal = mat3(matview) * normalize(normal * tbn);
	
	// convert to white + alpha
	vec4 color = vec4(vec3(1.0 - noise.a), 1.5 * noise.a);
	// sharply reduce left-over alpha
	color.a = pow(color.a, 6.0);
	if (color.a < 0.05) discard;
	color.a = smoothstep(0.0, 1.0, color.a);
	
	// lighten the insides a bit
	color.rgb = min(vec3(1.0), color.rgb * 1.9);
	color.rgb = pow(color.rgb, vec3(0.2));
	
	// blue
	color.b += (1.0 - color.b) * 0.15;
	
	// darken parts dense to player
	float away = -dot( reflect(v_eye, normal), v_ldir );
	color.rgb += min(0.0, away * 0.25);
	
#endif
	
	const float fadedist = maxdist * 0.9;
	float depthalpha = max(0.0, vertdist - fadedist);
	depthalpha = 1.0 - depthalpha / (maxdist - fadedist);
	color.a *= depthalpha * depthalpha; //don't preserve alpha
	
	// daylight multiplier
	color *= vec4(vec3(daylight * daylight), daylight);
	
	// sun -> clouds
	//vec3 sunBaseColor = color.rgb * vec3(1.0, 0.9, 0.8);
	//float sunAmount = max( -dot( v_eye, v_ldir ), 0.0 ) * 1.3;
	
	// sun -> clouds
	//color.rgb = mix(color.rgb, sunBaseColor, pow(sunAmount, 4.0) * 0.3);
	
	#ifdef POSTPROCESS
		gl_FragData[0] = color;
	#else
		gl_FragData[0] = vec4(pow(color.rgb, vec3(2.2)), color.a);
	#endif
}

#endif

