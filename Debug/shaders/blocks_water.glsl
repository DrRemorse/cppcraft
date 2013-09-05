#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM
#define NOISE_WATER
precision mediump float;

#ifdef VERTEX_PROGRAM
uniform mat4 matproj;
uniform mat4 matview;
uniform vec3 vtrans;

uniform vec3 worldOffset;
uniform vec3 lightVector;
uniform float frameCounter;

in vec3 in_vertex;
in vec3 in_normal;
in vec3 in_tangent;
in vec3 in_texture;
in vec4 in_color;
in vec4 in_color2;

out vec4 lightdata;
out vec4 torchlight;

out float vertdist;
out vec3 v_ldir;
out vec3 v_half;
out vec3 v_eye;
out vec3 v_normal;
out vec3 l_normal;
out vec3 l_reflect;
out mat3 tbn;

out vec4 waves;

const float VERTEX_SCALE

void main(void)
{
	vec4 position = vec4(in_vertex / VERTEX_SCALE + vtrans, 1.0);
	position = matview * position;
	vertdist = length(position);
	gl_Position = matproj * position;
	
	// light and eye direction in view space
	v_eye = normalize(-position.xyz);
	v_ldir = mat3(matview) * lightVector;
	v_half = normalize(v_eye + v_ldir);
	// reflect light in view space using view-normal
	l_normal = in_normal;
	v_normal = mat3(matview) * in_normal;
	
	// local reflection vector
	l_reflect = reflect(-v_eye, v_normal) * mat3(matview);
	// galactic coordinates
	vec3 w_vertex  = position.xyz * mat3(matview) - worldOffset;
	
#ifdef NOISE_WATER
	// Current water vertex position, scaled down a lot, then motion added in one direction	
	// output to fragment shader
	float timer = frameCounter / 35.0;
	
	waves.xy  = w_vertex.xz * vec2(0.10, 0.02) + timer * vec2(0.2, 0.0);
	waves.zw  = w_vertex.xz * vec2(0.25, 0.05) + timer * vec2(0.25, 0.05);
	
#else
	// texture to local space matrix
	vec3 binormal = cross(in_normal, in_tangent);
	tbn = mat3(in_tangent, binormal, in_normal);
	
	// output to fragment shader
	waves.xy  = w_vertex.zx * vec2( 0.005, 0.01)  + vec2(0.0, frameCounter / 5000.0);
	waves.zw  = w_vertex.zx * vec2(0.0125, 0.025) + vec2(0.0, frameCounter / 1000.0);
	
#endif
	
	lightdata   = in_color;
	torchlight  = in_color2;
}

#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable
precision highp float;

uniform sampler2D underwatermap;
uniform sampler2D wavenormals;
uniform samplerCube skymap;

uniform mat4 matview;

uniform vec3  screendata;
uniform float daylight;
uniform vec4  playerLight;
uniform float modulation;

in vec4 lightdata;
in vec4 torchlight;

in float vertdist;
in vec3 v_ldir;
in vec3 v_half;
in vec3 v_eye;
in vec3 v_normal;
in mat3 tbn; // tangent space

in vec3 l_normal;
in vec3 l_reflect;
in vec4 waves; // wave positions

const float ZFAR

#ifdef NOISE_WATER
#include "srdnoise.glsl"
#endif
#include "stdfog.glsl"

void main(void)
{
#ifdef NOISE_WATER
	// derivative simplex noise
	vec2 grad, grad2;
	srdnoise(waves.xy, 0.0, grad);
	srdnoise(waves.zw, 0.0, grad2);
	
	// final x/z values
	grad = grad * 0.7 + grad2 * 0.3; // + grad3 * 0.3;
	
	vec3 tx = vec3(0.0, 1.0, grad.x);
	vec3 ty = vec3(1.0, 0.0, grad.y);
	
	vec3 Normal = cross(tx, ty);
	Normal.y += 1.00001; // make sure its positively pointing upwards
	Normal = normalize(Normal);
	
	vec3 vNormal = mat3(matview) * Normal;
	vec3 viewNormal = normalize(v_normal + vNormal * 0.1);
	
	vec3 vReflect   = l_reflect + Normal * 0.125;
	
#else
	// normal-mapped (tangent space) noise
	
	// read from normal in tangent-space
	vec3 n1 = texture2D(wavenormals, waves.xy).rgb * 2.0 - vec3(1.0);
	vec3 n2 = texture2D(wavenormals, waves.zw).rgb * 2.0 - vec3(1.0);
	vec3 Normal = n1 + n2; //mix(n1, n2, 0.5);
	// rotate wavenormals from tangent space to world space
	Normal = normalize(tbn * Normal);
	// rotate world space normal to camera (view) space
	vec3 vNormal = mat3(matview) * Normal;
	
	vec3 viewNormal = normalize(v_normal + vNormal * 0.5);
	vec3 vReflect   = l_reflect + Normal * 0.15;
	
#endif
	
	vec3 vEye   = normalize(v_eye);
	vec3 vLight = normalize(v_ldir);
	
	//----- fresnel term -----
	
	float fresnel = dot(vEye, viewNormal);
	float fdepth = -min(fresnel, 0.0);
	fresnel = pow(1.0 - abs(fresnel), 5.0);
	
	//----- REFRACTION -----
	
	// underwatermap coordinates
	vec2 refcoord = gl_FragCoord.xy / screendata.xy;
	refcoord.y -= viewNormal.z * fdepth * 0.25 * (1.0 - refcoord.y * refcoord.y);
	refcoord.xy += Normal.xz * 0.0025;
	
	// start with underwater texture
	vec4 color = texture2D(underwatermap, refcoord.xy);
	// apply gamma ramp
	color.rgb = pow(color.rgb, vec3(2.2));
	
	// mix in seacolor based on 'depth'
	const vec3 deepwater = vec3(0.02, 0.10, 0.20);
	const vec3 shallowwater = vec3(0.2, 0.3, 0.2);
	
	float dep = length(color) * 0.6;
	color.rgb *= pow(vec3(dep), vec3(2.0, 1.4, 1.0));
	
	vec3 waterColor = mix(deepwater, shallowwater, dep * dep);
	color.rgb = mix(waterColor, color.rgb, dep);
	
	// fettskit
	//color *= max(0.25, 1.0 - fresnel);
	
	//----- REFLECTIONS -----
	
	// mix in sky reflection
	vec3 reflection = textureCube(skymap, vReflect).rgb * daylight;
	color.rgb = mix(color.rgb, reflection, 0.1 + 0.6 * fresnel);
	
	// fake waves
	color.rgb *= 1.0 + (dot(Normal, l_normal) + 0.75) * 0.1;
	
	// degamma
	color.rgb = pow(color.rgb, vec3(1.0 / 2.2));
	
	//- lighting (we need shadow later) -//
	#include "lightw.glsl"
	
	// sun / specular
	const vec3  SUNCOLOR = vec3(1.0, 0.7, 0.5);
	const float SUNSPEC  = 0.7; // specular
	const float SUNSHINE = 3.0; // shininess
	
	float shine = max(0.0, dot(viewNormal, normalize(v_half)) );
	float spec = max(0.0, dot(reflect(-vLight, viewNormal), vEye));
	
	vec3 specular = SUNCOLOR * SUNSPEC * pow(spec, 16.0) + pow(spec, 4.0) * 0.2;
	color.rgb += specular * pow(shine, SUNSHINE) * daylight * shadow * shadow;
	
#ifdef SUPERFOG
	// super fog
	float fogdist = 1.0 - vertdist / ZFAR;
	superFog(color.rgb, fogdist, vEye, vLight);
#endif
	
	#include "horizonfade.glsl"
	
	// final color
	#include "finalcolor.glsl"
}

#endif
