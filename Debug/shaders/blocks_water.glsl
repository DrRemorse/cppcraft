#version 150
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#define REFLECTIONS

#ifdef VERTEX_PROGRAM
uniform mat4 matproj;
uniform mat4 matview;
uniform vec3 vtrans;

uniform vec3 worldOffset;
uniform vec3 lightVector;
uniform float frameCounter;

in vec4 in_vertex;
in vec4 in_normal;
in vec3 in_texture;
in vec4 in_biome;
in vec4 in_color;
in vec4 in_color2;

out vec4 waterColor;
out vec4 lightdata;
out vec4 torchlight;

out vec3 v_pos;
out vec3 v_ldir;
out vec3 v_normal;
out vec4 waves;

const float VERTEX_SCALE_INV

void main(void)
{
	vec4 position = vec4(in_vertex.xyz * VERTEX_SCALE_INV, 1.0);
	position.xyz += vtrans;
	position = matview * position;
	gl_Position = matproj * position;
	
	// light and eye direction in view space
	v_pos = -position.xyz;
	v_ldir = mat3(matview) * lightVector;
	// reflect light in view space using view-normal
	v_normal = mat3(matview) * in_normal.xyz;
	
	// galactic coordinates
	vec3 w_vertex  = position.xyz * mat3(matview) - worldOffset;
	
	// Current water vertex position, scaled down a lot, then motion added in one direction	
	// output to fragment shader
	float timer = frameCounter / 35.0;
	
	waves.xy = w_vertex.xz * vec2(0.10, 0.02) + timer * vec2(0.2, 0.0);
	waves.zw = w_vertex.xz * vec2(0.25, 0.05) + timer * vec2(0.2, 0.0);
	
	waterColor  = in_biome;
	lightdata   = in_color;
	torchlight  = in_color2;
}

#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable
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

in vec3 v_ldir;
in vec3 v_pos;
in vec3 v_normal;

in vec4 waves; // wave positions
const float ZFAR

out vec4 color;

#include "srdnoise.glsl"

void main(void)
{
	// derivative simplex noise
	vec2 grad, grad2;
	srdnoise(waves.xy, 0.0, grad);
	srdnoise(waves.zw, 0.0, grad2);
	
	vec3 tx = vec3(0.0, grad.x, 2.0);
	vec3 tz = vec3(2.0, 0.0, grad.y);
	vec3 n1 = cross(tx, tz);
	
	tx = vec3(0.0, grad2.x, 2.0);
	tz = vec3(2.0, 0.0, grad2.y);
	vec3 n2 = cross(tx, tz);
	
	// average the gradients
	vec3 Normal = normalize(n1 + n2);
	
	/*
	const vec3 norm = vec3(0, 1, 0);
	const vec3 tang = vec3(1, 0, 0);
	vec3 binorm = cross(tang, norm);
	
	mat3 tbn = mat3(tang.x, binorm.x, norm.x,
					tang.y, binorm.y, norm.y,
					tang.z, binorm.z, norm.z);
	
	vec3 n1 = texture(dudvmap, waves.yx).rgb * 2.0 - vec3(1.0);
	vec3 n2 = texture(dudvmap, waves.wz).rgb * 2.0 - vec3(1.0);
	vec3 Normal = n1 + n2;
	Normal = normalize(Normal) * tbn;
	*/
	
	float vertdist = length(v_pos);
	
	vec3 vNormal = mat3(matview) * Normal;
	vec3 viewNormal = normalize(v_normal);
	
	// normalize inputs (water planes are complex)
	vec3 vEye   = v_pos / vertdist;
	vec3 vLight = normalize(v_ldir);
	vec3 vHalf = normalize(vEye + vLight);
	
	// screenspace tex coords
	vec2 texCoord = gl_FragCoord.xy / screendata.xy;
	// normalized distance
	float dist = vertdist / ZFAR;
	
	//----- REFRACTION -----
	
	// wave modulation
	vec2 refcoord = texCoord + vNormal.xz * (0.003 + 0.02 * dist);
	
	// read underwater, use as base color
	vec4 underw = texture(underwatermap, refcoord);
	float wdepth = underw.a - dist;
	
	// COSTLY re-read to avoid reading inside terrain
	if (wdepth < 0.0)
	{
		underw = texture(underwatermap, texCoord);
		wdepth = underw.a - dist;
	}
	
	// nicer depth, adding a little extra
	wdepth += 0.02; //max(0.0, 0.02 + wdepth);
	
	// above water we want the sky to appear as 100% seafloor
	float dep = 1.0 - smoothstep(0.0, 0.04, wdepth);
	float depthTreshold = min(1.0, wdepth * 12.0);
	
	//----- SEACOLOR -----
	const vec3 deepWater    = vec3(42, 73, 87) * vec3(1.0 / 255.0);
	const vec3 shallowWater = vec3(0.35, 0.55, 0.50);
	
	// create final water color
	color = vec4(mix(shallowWater, deepWater, depthTreshold), 1.0);
	
	// mix watercolor and refraction/seabed
	color.rgb = mix(color.rgb, underw.rgb, dep);
	
	//----- REFLECTIONS -----
	
#ifdef REFLECTIONS
	// world/terrain reflection
	vec3 wreflection = texture(reflectionmap, refcoord).rgb;
	
	//----- fresnel term -----
	float fresnel = max(0.0, dot(vEye, viewNormal));
	fresnel = pow(1.0 - fresnel, 3.0);
	
	// add reflections to final color
	color.rgb = mix(color.rgb, wreflection, fresnel * 0.75);
	//color.rgb = vec3(fresnel);
#endif
	
	//- lighting -//
	#include "lightw.glsl"
	
	//- sun/specular -//
	const vec3  SUNCOLOR = vec3(0.9, 0.7, 0.5);
	
	float shine = max(0.0, dot(vHalf, viewNormal) );
	float specf = max(0.0, dot(reflect(-vLight, viewNormal), vEye));
	float spec  = dot(reflect(-vLight, vNormal), vEye);
	float specv = max(0.0, spec);
	
	// ocean waves + sun highlights
	vec3 highlights = pow(specv, 20.0) * vec3(0.4) + spec * 0.04;
	color.rgb += highlights * shadow * shadow;
	
	// shiny/specular sun reflection
	vec3 specular = SUNCOLOR * pow(specf, 20.0) * 1.5 * pow(shine, 8.0);
	color.rgb += specular * shadow * shadow;
	
	//- distance->alpha -//
	#include "horizonfade.glsl"
	
	// final color
	#include "finalcolor.glsl"
}

#endif
