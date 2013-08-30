#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM
in  vec3 in_vertex;
out vec2 texCoord;

void main(void)
{
	texCoord = in_vertex.xy;
	gl_Position = vec4(in_vertex.xy * 2.0 - 1.0, 0.0, 1.0);
}
#endif

#ifdef FRAGMENT_PROGRAM
uniform sampler2D terrain;
uniform sampler2D depthtexture;
uniform sampler2D blurtexture;
uniform sampler2D skytexture;

in vec2 texCoord;

float linearizeDepth(float Z);
const float ZFAR
const float ZNEAR

void main()
{
	float depth = texture2D(depthtexture, texCoord).x;
	depth = linearizeDepth(depth);
	
	// base color
	const float BLURMULT    = 1.0;
	float mixAmount = smoothstep(0.2, 0.8, depth) * BLURMULT;
	
	vec4 color = texture2D(terrain, texCoord);
	color = mix(color, texture2D(blurtexture, texCoord), mixAmount);
	
	// fade out world based on depth
	float fade = 1.0 - color.a;
	
	vec3 skyColor = texture2D(skytexture, texCoord).rgb;
	color.rgb = mix(color.rgb, skyColor, fade);
	
	gl_FragData[0] = vec4(color.rgb, fade);
}

float linearizeDepth(float Z)
{
	float depth = 2 * Z - 1;
	
	// linearize to [0, 1]
	return 2.0 * ZNEAR / (ZFAR + ZNEAR - depth * (ZFAR - ZNEAR)); // * ZFAR
}

#endif
