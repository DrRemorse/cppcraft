#version 150
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#define POSTPROCESS
const float ZFAR
const float SKYFAR = ZFAR * 1.6;

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

void main()
{
	vec4 position = matview * vec4(in_vertex, 1.0);
	
	mat3 matrot = mat3(matview);
	vec3 lpos = position.xyz * matrot;
	// new vertex distance
	vertdist = min(SKYFAR, length(lpos));
	
	v_ldir = mat3(matview) * lightVector;
	v_eye = -position.xyz;
	
	texCoord = (lpos.xz - worldOffset.xz) / 48.0 / 8.0;
	// movement speed
	texCoord.s += frameCounter * 0.0001;
	
	gl_Position = matproj * position;
}

#endif

#ifdef FRAGMENT_PROGRAM
uniform sampler2D cloudstex;

uniform float frameCounter;
uniform float daylight; // multiplier

uniform mat4 matview;

in vec2 texCoord;
in float vertdist;

in  vec3 v_ldir;
in  vec3 v_eye;
out vec4 color;

void main(void)
{
	vec3 vEye = normalize(v_eye);
	
	// textured clouds
	vec4 noise = texture(cloudstex, texCoord);
	vec3 normal = noise.rgb * 2.0 - vec3(1.0);
	
	const vec3 in_normal  = vec3( 0.0,  1.0,  0.0);
	const vec3 in_tangent = vec3( 1.0,  0.0,  0.0);
	const vec3 binormal   = vec3( 0.0,  0.0, -1.0);
	
	const mat3 tbn = mat3(in_tangent, binormal, in_normal);
	normal = mat3(matview) * normalize(normal * tbn);
	
	// convert to white + alpha
	color = vec4(vec3(1.0 - noise.a), 1.5 * noise.a);
	// sharply reduce left-over alpha
	color.a = pow(color.a, 6.0);
	//if (color.a < 0.05) discard;
	color.a = smoothstep(0.0, 1.0, color.a);
	
	// lighten the insides a bit
	color.rgb = min(vec3(1.0), color.rgb * 1.9);
	color.rgb = pow(color.rgb, vec3(0.2));
	
	// blue
	color.b += (1.0 - color.b) * 0.15;
	
	// darken parts dense to player
	float away = -dot( reflect(vEye, normal), v_ldir );
	color.rgb += min(0.0, away * 0.25);
	
	// fade out to distance
	const float fadedist = SKYFAR * 0.5;
	
	float depthalpha = max(0.0, vertdist - fadedist);
	depthalpha = 1.0 - depthalpha / (SKYFAR - fadedist);
	
	color.a *= depthalpha;
	
	// sun -> clouds
	vec3 sunBaseColor = color.rgb * vec3(1.0, 0.9, 0.8);
	float sunAmount = max( -dot( vEye, v_ldir ), 0.0 ) * 1.3;
	color.rgb = mix(color.rgb, sunBaseColor, pow(sunAmount, 4.0) * 0.3);
	
	// daylight multipliers
	color.rgb *= daylight * daylight;
	color.a   *= daylight;
	
#ifndef POSTPROCESS
	color = vec4(pow(color.rgb, vec3(2.2)), color.a);
#endif
}

#endif

