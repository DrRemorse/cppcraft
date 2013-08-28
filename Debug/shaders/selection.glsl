#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

precision highp float;

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
const float visibility     = 0.25;

in vec2 texCoord;

void main(void)
{
	/*
	const float BORDER = 0.90;
	
	float dist = max(abs(cheby.x), abs(cheby.y));
	if (dist < BORDER) discard;
	dist = (dist - BORDER) / (1.0 - BORDER);
	dist = pow(dist, 16.0);
	
	gl_FragColor = vec4(selectionColor, visibility * dist);
	*/
	
	vec4 color = texture2D(texture, texCoord);
	
	gl_FragColor = vec4(selectionColor, color.x * visibility);
	
}
#endif
