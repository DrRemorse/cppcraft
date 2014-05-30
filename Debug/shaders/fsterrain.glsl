#version 150
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
uniform sampler2D terrain;
uniform sampler2D blurtexture;

in  vec2 texCoord;
out vec4 color;

void main()
{
	// terrain color
	color = texture2D(terrain, texCoord);
	
	// blurred color
	vec4 blur = texture2D(blurtexture, texCoord);
	// depth from blur
	float depth = blur.a * step(blur.a, 0.996);
	
	// blur distance
	float mixAmount = min(1.0, depth / 0.9);
	color = mix(color, blur, mixAmount);
}

#endif
