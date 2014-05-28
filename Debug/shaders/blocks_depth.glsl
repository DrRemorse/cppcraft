#version 330
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM
precision mediump float;

uniform mat4 matproj;
uniform mat4 matview;
uniform vec3 vtrans;

in vec3 in_vertex;
in vec3 in_normal;
in vec3 in_texture;
in vec4 in_biome;
in vec4 in_color;
in vec4 in_color2;

out vec3 pos;
const float VERTEX_SCALE

void main(void)
{
	vec4 position = vec4(in_vertex / VERTEX_SCALE + vtrans, 1.0);
	position = matview * position;
	//vertdist = length(position.xyz) / ZFAR;
	gl_Position = matproj * position;
	
	pos = position.xyz;
}

#endif

#ifdef FRAGMENT_PROGRAM

uniform sampler2D texture;
uniform vec3 screensize;

in vec3 pos;
const float ZFAR

void main(void)
{
	float vertdist = length(pos) / ZFAR;
	
	vec2 texCoord =  gl_FragCoord.xy / screensize.xy;
	gl_FragData[0] = vec4(texture2D(texture, texCoord).rgb, vertdist);
}

#endif
