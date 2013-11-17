#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM

uniform mat4 mvp;

in vec3 in_vertex;
in vec3 in_texture;

out vec3 texCoord;

void main(void)
{
	texCoord = in_texture;
	
	gl_Position = mvp * vec4(in_vertex.xyz, 1.0);
}

#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DArray texture;

in vec3 texCoord;

void main(void)
{
	vec4 color = texture2DArray(texture, texCoord.stp);
	
	gl_FragData[0] = color;
}

#endif

