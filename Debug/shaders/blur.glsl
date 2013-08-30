#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM
in  vec2 in_vertex;
out vec2 texCoord;

void main(void)
{
	texCoord = in_vertex;
	gl_Position = vec4(in_vertex * 2.0 - 1.0, 0.0, 1.0);
}
#endif

#ifdef FRAGMENT_PROGRAM
uniform sampler2D texture;
uniform int Width; // samples in each direction
uniform vec2 dir;   // (1 / w, 0.0) etc.

in vec2 texCoord;

void main()
{
	vec4 color = vec4(0.0);
	float kernel = float(Width + 1);
	float width, sum = 0.0;
	
	for(int i = -Width; i <= Width; i++)
	{
		width = kernel - abs(float(i));
		color += texture2D(texture, texCoord + float(i) * dir) * width;
		sum += width;
	}
	
	gl_FragData[0] = color / sum;
}
#endif
