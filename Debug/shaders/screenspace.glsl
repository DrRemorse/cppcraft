#version 130
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
uniform sampler2D texture;
#ifdef LENSFLARE
uniform sampler2D lensflare;
#endif
uniform float frameCounter;
uniform int   submerged;

in vec2 texCoord;

const vec3 SUB_WATER = vec3(0.01, 0.08, 0.3);
const vec3 SUB_LAVA  = vec3(0.28, 0.06, 0.0);

const float ZFAR
const float ZNEAR

const float A = 0.15;
const float B = 0.50;
const float C = 0.10;
const float D = 0.20;
const float E = 0.02;
const float F = 0.30;
const float W = 1.25;

vec3 Uncharted2Tonemap(vec3 x)
{
	return ((x * (A*x+C*B) + D * E) / (x * (A*x+B) + D * F)) - E / F;
}

void main()
{
	vec4 color = texture2D(texture, texCoord);
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
		
		color = texture2D(texture, texCoord + waves);
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
	
	color.rgb = pow(color.rgb, vec3(2.2));
	
#ifdef LENSFLARE
	// add lens flare & dirt
	color.rgb += texture2D(lensflare, texCoord).rgb;
#endif
	
	/*
	// filmic exposure filter
	float expmult = 0.6 + 0.5 * length(luminance) / 1.73;
	float expbias = 1.0;
	
	color *= expmult; // exposure multiplier
	vec3 curr = Uncharted2Tonemap(color * expbias);
	
	const vec3 wsColor = vec3(W);
	vec3 whiteScale = 1.0 / Uncharted2Tonemap(wsColor);
	color = curr * whiteScale;
	
	// contract
	//color = pow(color, vec3(1.0/2.2));
	*/
	
	gl_FragData[0] = color;
}

#endif
