#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM
precision mediump float;

#ifdef VERTEX_PROGRAM
uniform mat4 matmvp;
uniform vec3 vtrans;
uniform float miningTile;

in vec3 in_vertex;
in vec2 in_texture;

out vec3 texCoord;

void main()
{
	vec4 position = vec4(in_vertex.xyz + vtrans, 1.0);
	
	texCoord = vec3(in_texture, miningTile);
	
	gl_Position = matmvp * position;
}
#endif

#ifdef FRAGMENT_PROGRAM
#extension GL_EXT_gpu_shader4 : enable
uniform sampler2DArray texture;

in vec3 texCoord;

void main(void)
{
	vec4 color = texture2DArray(texture, texCoord);
	gl_FragData[0] = vec4(color);
}
#endif
