#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM
#define HIGHQ_BLUR
#define LENSFLARE

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
uniform sampler2D texture;
uniform sampler2D depthtexture;
#ifdef LENSFLARE
uniform sampler2D lensflare;
#endif
#ifdef HIGHQ_BLUR
uniform sampler2D blurtexture;
#endif
uniform int submerged;

in vec2 texCoord;

const vec3 SUB_WATER = vec3(0.01, 0.08, 0.3);
const vec3 SUB_LAVA  = vec3(0.28, 0.06, 0.0);

const float MOTION_BLUR = 0.225;
const float FAR_DEPTH   = 0.8;

float linearizeDepth(vec2 coord);

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
	float depth = linearizeDepth(texCoord);
	// avoid blurring sky/atmosphere
	if (depth > 0.99) depth = 0.65;
	
#ifdef HIGHQ_BLUR
	const float BLURMULT    = 1.0;
	float mixAmount = smoothstep(0.2, 1.0, depth) * BLURMULT;
	
	vec3 color = mix(texture2D(texture, texCoord).rgb, texture2D(blurtexture, texCoord).rgb, mixAmount);
#else
	const float BLURMULT    = 2.0;
	float mixAmount = smoothstep(0.2, 0.9, depth * depth) * BLURMULT;
	
	vec3 color = texture2DLod(texture, texCoord, mixAmount).rgb;
#endif
	
	if (submerged == 1)
	{	// submerged in water
		color = mix(color, SUB_WATER, 0.4 + 0.6 * smoothstep(0.0, 0.15, depth));
	}
	else if (submerged == 2)
	{	// submerged in lava
		color = mix(color, SUB_LAVA, 0.6 + 0.4 * smoothstep(0.0, 0.1, depth));
	}
	
	color = pow(color, vec3(2.2));
	
#ifdef LENSFLARE
	// add lens flare & dirt
	color += texture2D(lensflare, texCoord).rgb;
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
	
	gl_FragData[0] = vec4(color, 1.0);
}

float linearizeDepth(vec2 coord)
{
	float depth = 2 * texture2D(depthtexture, coord).r - 1;
	
	// linearize to [0, 1]
	return 2.0 * ZNEAR / (ZFAR + ZNEAR - depth * (ZFAR - ZNEAR)); // * ZFAR
}

#endif
