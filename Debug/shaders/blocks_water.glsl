#version 330
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#define REFLECTIONS

#ifdef VERTEX_PROGRAM
//precision highp float;
precision mediump float;

uniform mat4 matproj;
uniform mat4 matview;
uniform vec3 vtrans;

uniform vec3 worldOffset;
uniform vec3 lightVector;
uniform float frameCounter;

in vec3 in_vertex;
in vec3 in_normal;
in vec3 in_texture;
in vec4 in_biome;
in vec4 in_color;
in vec4 in_color2;

out vec4 waterColor;
out vec4 lightdata;
out vec4 torchlight;

out float vertdist;
out vec3 v_eye;
out vec3 v_ldir;
out vec3 v_half;
out vec3 v_normal;
flat out vec3 l_normal;

out vec4 waves;

const float VERTEX_SCALE

void main(void)
{
	vec4 position = vec4(in_vertex / VERTEX_SCALE + vtrans, 1.0);
	position = matview * position;
	vertdist = length(position.xyz);
	gl_Position = matproj * position;
	
	// light and eye direction in view space
	v_eye = -position.xyz / vertdist;
	v_ldir = mat3(matview) * lightVector;
	v_half = normalize(v_eye + v_ldir);
	// reflect light in view space using view-normal
	l_normal = in_normal;
	v_normal = mat3(matview) * in_normal;
	
	// galactic coordinates
	vec3 w_vertex  = position.xyz * mat3(matview) - worldOffset;
	
	// Current water vertex position, scaled down a lot, then motion added in one direction	
	// output to fragment shader
	float timer = frameCounter / 35.0;
	
	waves.xy  = w_vertex.xz * vec2(0.10, 0.02) + timer * vec2(0.2, 0.0);
	waves.zw  = w_vertex.xz * vec2(0.25, 0.05) + timer * vec2(0.2, 0.0);
	
	waterColor  = in_biome;
	lightdata   = in_color;
	torchlight  = in_color2;
}

#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable
//precision highp float;

uniform sampler2D underwatermap;
uniform sampler2D reflectionmap;

uniform mat4 matview;
uniform vec3 screendata;

uniform float frameCounter;
uniform float daylight;
uniform vec4  playerLight;
uniform float modulation;

in vec4 waterColor;
in vec4 lightdata;
in vec4 torchlight;

in float vertdist;
in vec3 v_ldir;
in vec3 v_half;
in vec3 v_eye;
in vec3 v_normal;

flat in vec3 l_normal;
in vec4 waves; // wave positions

const float ZFAR

#include "srdnoise.glsl"

void main(void)
{
	// derivative simplex noise
	vec2 grad, grad2;
	srdnoise(waves.xy, 0.0, grad);
	srdnoise(waves.zw, 0.0, grad2);
	
	// final x/z values
	grad = grad * 0.5 + grad2 * 0.5;
	
	vec3 tx = vec3(1.0, 0.0, grad.x);
	vec3 ty = vec3(0.0, 1.0, grad.y);
	
	vec3 Normal = cross(ty, tx);
	Normal.y += 1.0; // make sure its extremely positive
	Normal = normalize(Normal);
	
	vec3 vNormal = mat3(matview) * Normal;
	vec3 viewNormal = normalize(v_normal);
	
	
	//#define vEye   v_eye
	//#define vLight v_ldir
	vec3 vEye   = normalize(v_eye);
	vec3 vLight = normalize(v_ldir);
	
	//----- fresnel term -----
	
	float fresnel = dot(vEye, viewNormal);
	fresnel = pow(1.0 - max(0.0, fresnel), 5.0);
	
	//----- REFRACTION -----
	
	// screenspace tex coordinates
	vec2 texCoord = gl_FragCoord.xy / screendata.xy;
	
	// start with underwater texture
	float dist = vertdist / ZFAR;
	
	// wave modulation
	vec2 refcoord = texCoord + Normal.xz * 0.01 * (0.5 + 0.5 * dist);
	
	// read underwater, use as base color
	vec4 underw = texture2D(underwatermap, refcoord);
	
	//----- REFLECTIONS -----
	
#ifdef REFLECTIONS
	// world/terrain reflection
	vec4 wreflection = texture2D(reflectionmap, refcoord);
#endif
	
	//----- SEACOLOR -----
	
	const vec3 deepwater    = vec3(0.26, 0.44, 0.53);
	const vec3 shallowwater = vec3(0.35, 0.55, 0.60);
	
	// see above:
	float wdepth = 0.02 + max(0.0, underw.a - dist);
	
	// above water we want the sky to appear as 100% seafloor
	float dep = 1.0 - smoothstep(0.0, 0.04, wdepth);
	
	// create final water color
	vec4 color = vec4(mix(shallowwater, deepwater, wdepth * 4.0), 1.0);
	// mix water color and other-side
	color.rgb = mix(color.rgb, underw.rgb, dep); // * daylight;
	
	//color.rgb = vec3(wdepth * 5.0);
	
#ifdef REFLECTIONS
	// add reflections
	color.rgb = mix(color.rgb, wreflection.rgb, fresnel);
#endif
	
	// fake waves
	float wavereflect = dot(reflect(-vLight, vNormal), vEye);
	color.rgb *= 1.0 + wavereflect * 0.1 * daylight;
	
	//- lighting (we need shadow later) -//
	#include "lightw.glsl"
	
	// sun / specular
	const vec3  SUNCOLOR = vec3(1.0, 0.57, 0.23);
	const float SUNSPEC  = 0.55; // specular
	const float SUNSHINE = 2.0; // shininess
	
	float shine = max(0.0, dot(viewNormal, normalize(v_half)) );
	float spec  = max(0.0, dot(reflect(-vLight, viewNormal), vEye));
	
	vec3 specular = SUNCOLOR * SUNSPEC * pow(spec, 16.0) + pow(spec, 4.0) * 0.3;
	color.rgb += specular * pow(shine, SUNSHINE) * daylight * shadow * shadow;
	
	#include "horizonfade.glsl"
	
	// final color
	#include "finalcolor.glsl"
}

#endif
