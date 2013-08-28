#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM
uniform mat4 matproj;
uniform mat4 matview;
uniform mat4 matrot;

uniform vec3 lightVector;
uniform float handTileID;

in vec3 in_vertex;
in vec3 in_normal;
in vec2 in_texture;

out vec3 texCoord;
flat out float worldLight;
flat out float brightness;

void main()
{
	texCoord = vec3(in_texture, handTileID);
	vec3 normal = in_normal * mat3(matrot);
	
	float dotl = dot(normal, lightVector);
	
	worldLight  = 0.75;
	worldLight += dotl * (1.0 - worldLight);
	
	gl_Position = matproj * matview * vec4(in_vertex, 1.0);
}
#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DArray texture;

uniform float daylight;
uniform vec4  lightdata;
uniform vec4  torchlight;
uniform float modulation;

in vec3 texCoord;
flat in float worldLight;
flat in float brightness;

void main(void)
{
	vec3 color = texture2DArray(texture, texCoord).rgb;
	
	#include "degamma.glsl"
	
	/// shadows & torchlight ///
	float brightness = lightdata.y * modulation;
	// increase brightness if player holds an emissive item
	//#include "playerlight.glsl"

	// shadow is smallest between shadow-value and daylight level
	float shadow = min(1.0, min(daylight, lightdata.x) + brightness);

	// scaled shadow color
	vec3 shadowColor = vec3(-0.2, 0.0, 0.2) * shadow;

	color.rgb *= worldLight;
	// mix in shadows
	color.rgb = mix(shadowColor, color.rgb, shadow);
	// mix in torchlight
	color.rgb = mix(color.rgb, torchlight.rgb, torchlight.a * modulation);
	
	const vec3 gamma = vec3(2.2);
	
	// back to gamma space
	gl_FragColor = vec4(pow(color.rgb, gamma), 1.0);
}

#endif
