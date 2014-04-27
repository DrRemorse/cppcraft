#version 330
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM
uniform mat4 matproj;
uniform mat4 matview;
uniform vec3 vtrans;

uniform vec3 worldOffset;

in vec3 in_vertex;
in vec3 in_normal;
in vec3 in_texture;
in vec4 in_biome;
in vec4 in_color;
in vec4 in_color2;

out float vertdist;
out vec2  wave;

const float VERTEX_SCALE
const float ZFAR

void main(void)
{
	vec4 position = vec4(in_vertex / VERTEX_SCALE + vtrans, 1.0);
	position = matview * position;
	vertdist = length(position.xyz) / ZFAR;
	gl_Position = matproj * position;
	
	// transformed wave coordinates
	wave = (position.xyz * mat3(matview) - worldOffset).xz;
}

#endif

#ifdef FRAGMENT_PROGRAM
uniform sampler2D underwater;
uniform sampler2D lavatex;
uniform vec3 screendata;

uniform float frameCounter;

in float vertdist;
in vec2  wave;

void main(void)
{
	float timer = frameCounter / 50.0;
	vec2 subwave  = sin(timer * vec2(2.0) + wave * 0.5) * sin(timer);
	vec2 lavawave = wave * 0.25 + subwave * 0.01;
	
	// sloppy waving
	vec3 lavaColor = texture2D(lavatex, lavawave).rgb;
	// cheap sparkling
	lavaColor.r += pow(lavaColor.r, 1.0 / abs(subwave.x * subwave.y)) * 0.5;
	
#ifdef UNDERLAVA
	// under the lava
	vec2 texCoord =  gl_FragCoord.xy / screendata.xy;
	vec4 underlava = texture2D(underwater, texCoord);
	
	float wdepth = 0.02 + max(0.0, underlava.a - vertdist);
	
	float dep = smoothstep(0.0, 0.03, wdepth);
	
	// mix it all together
	vec3 color = mix(underlava.rgb, lavaColor, dep);
#else
	vec3 color = lavaColor;
#endif
	
	gl_FragData[0] = vec4(color, vertdist);
}

#endif
