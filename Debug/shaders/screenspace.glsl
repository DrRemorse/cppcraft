#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM
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
uniform int submerged;

in vec2 texCoord;

const vec3 SUB_WATER = vec3(0.01, 0.08, 0.3);
const vec3 SUB_LAVA  = vec3(0.28, 0.06, 0.0);

const float MOTION_BLUR = 0.225;

float linearizeDepth(float Z);

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
	float depth = texture2D(depthtexture, texCoord).x;
	depth = linearizeDepth(depth);
	
	vec4 color = texture2D(texture, texCoord);
	
	if (submerged == 1)
	{	// submerged in water
		color.rgb = mix(color.rgb, SUB_WATER, 0.4 + 0.6 * smoothstep(0.0, 0.15, depth));
	}
	else if (submerged == 2)
	{	// submerged in lava
		color.rgb = mix(color.rgb, SUB_LAVA, 0.6 + 0.4 * smoothstep(0.0, 0.1, depth));
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
	
	gl_FragData[0] = vec4(color.rgb, 1.0);
}

float linearizeDepth(float Z)
{
	float depth = 2 * Z - 1;
	
	// linearize to [0, 1]
	return 2.0 * ZNEAR / (ZFAR + ZNEAR - depth * (ZFAR - ZNEAR)); // * ZFAR
}

#endif
