#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM
precision mediump float;

#ifdef VERTEX_PROGRAM
uniform mat4 matmvp;
uniform vec3 vtrans;

in vec3 in_vertex;
in vec2 in_texture;

out vec2 texCoord;

void main()
{
	vec4 position = vec4(in_vertex.xyz + vtrans, 1.0);
	
	texCoord = in_texture;
	
	gl_Position = matmvp * position;
}
#endif

#ifdef FRAGMENT_PROGRAM
uniform sampler2D texture;

const vec3  selectionColor = vec3(0.0);

in vec2 texCoord;

void main(void)
{
	float alpha = texture2D(texture, texCoord).r;
	
	gl_FragData[0] = vec4(selectionColor, alpha);
}
#endif
