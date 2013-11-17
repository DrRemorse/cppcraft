#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM
in  vec2 in_vertex;
out vec2 texCoord;

void main(void)
{
	texCoord = in_vertex;
	gl_Position = vec4(in_vertex * 2.0 - 1.0, 0.0, 1.0);
}
#endif

#ifdef FRAGMENT_PROGRAM
uniform sampler2D terrain;
uniform sampler2D skytexture;
//uniform sampler2D normals;
//uniform sampler2D noisetex;

uniform float daylight;
uniform float sundot;
uniform vec2  sunCoord;

in vec2 texCoord;

const float ZFAR
const float ZNEAR

void main()
{
	// base color
	vec4 color = texture2D(terrain, texCoord);
	// depth from alpha
	#define depth  color.a
	
	// add fog & sunlight
	float sunrad = 1.0 - distance(texCoord, sunCoord) / 0.5;
	
	const float FOG_MAX  = 0.3;
	const float SUN_MULT = 1.0;
	const vec3 fogBaseColor = vec3(0.75, 0.75, 0.8);
	const vec3 sunBaseColor = vec3(1.0, 0.9, 0.7);
	
	float sunAmount = max(0.0, sunrad * SUN_MULT);
	float fogAmount = smoothstep(0.0, 1.0, depth) * FOG_MAX;
	vec3  fogColor  = mix(fogBaseColor, sunBaseColor, sunAmount);
	
	float notSky = step(depth, 0.999);
	
	// SSAO
	//color.rgb = vec3(getSSAO(depth) * notSky);
	
	// disable fog for atmosphere/sky
	fogAmount *= notSky;
	// mix in fog
	color.rgb = mix(color.rgb, fogColor * daylight, fogAmount);
	
	// crossfade out world by blending with sky
	const float fade_dist = 0.90;
	float fade = max(0.0, (color.a - fade_dist) / (1.0 - fade_dist));
	
	vec3 skyColor = texture2D(skytexture, texCoord).rgb;
	color.rgb = mix(color.rgb, skyColor, fade);
	
	gl_FragData[0] = color;
}

#endif
