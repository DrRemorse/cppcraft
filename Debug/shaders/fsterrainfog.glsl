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
	float depth = color.a;
	
	// add fog & sunlight
	float sunrad = 1.0 - distance(texCoord, sunCoord) / 0.5;
	float notSky = step(depth, 0.998);
	
	const vec3 fogBaseColor = vec3(0.9);
	const vec3 sunBaseColor = vec3(1.0, 1.0, 0.7);
	
	float sunAmount = max(0.0, sunrad) * notSky;
	float fogAmount = smoothstep(0.5, 1.0, depth);
	color.rgb = mix(color.rgb, fogBaseColor, fogAmount * fogAmount);
	color.rgb = mix(color.rgb, sunBaseColor, sunAmount * fogAmount);
	
	// sun testing
	//color.rgb = vec3(sunAmount);
	
	// mix in fog
	vec3 skyColor = texture2D(skytexture, texCoord).rgb;
	color.rgb = mix(color.rgb, skyColor, fogAmount);
	
	gl_FragData[0] = color;
}

#endif
