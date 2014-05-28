#version 150
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM
#define LENSFLARE

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
uniform sampler2D lensflare;

uniform float frameCounter;
uniform int   submerged;

in  vec2 texCoord;
out vec4 color;

const vec3 SUB_WATER = vec3(0.01, 0.08, 0.3);
const vec3 SUB_LAVA  = vec3(0.28, 0.06, 0.0);

const float ZFAR
const float ZNEAR

void main()
{
	color = texture(terrain, texCoord);
	float depth = color.a;
	
	if (submerged != 0)
	{
		// re-read waved texcoord
		float speed = frameCounter * 0.01;
		
		// camera bobbing
		vec2 waves = vec2(sin(speed), cos(speed)) * 0.01;
		// screen waves
		vec2 waveCoords = vec2(speed * 5) + texCoord.xy * 6.28 * 32;
		float wavyDepth = smoothstep(0.0, 0.1, depth);
		waves += vec2(cos(waveCoords.x), sin(waveCoords.y)) * wavyDepth * 0.005;
		
		color = texture(terrain, texCoord + waves);
		depth = color.a;
		wavyDepth = smoothstep(0.0, 0.05, depth);
		
		if (submerged == 1)
		{	// submerged in water
			color.rgb = mix(color.rgb, SUB_WATER, 0.35 + 0.65 * wavyDepth);
		}
		else
		{	// submerged in lava
			color.rgb = mix(color.rgb, SUB_LAVA, 0.6 + 0.4 * wavyDepth);
		}
	}
	
	// fade-in
	/*
	const vec3 luma = vec3(0.299, 0.587, 0.114);
	float luminance = dot(luma, color.rgb);
	float fadein    = min(1.0, frameCounter / 250.0);
	
	color.rgb = mix(vec3(luminance), color.rgb, fadein);
	*/
	
	// degamma ramp
	color.rgb = pow(color.rgb, vec3(2.2));
	
#ifdef LENSFLARE
	// add lens flare & dirt
	color.rgb += texture(lensflare, texCoord).rgb;
#endif
	
}

#endif
