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
uniform sampler2D terrain;
uniform sampler2D blurtexture;

in vec2 texCoord;

const float ZFAR
const float ZNEAR

void main()
{
	// base color
	vec4 color = texture2D(terrain, texCoord);
	// blurred color
	vec4 blur = texture2D(blurtexture, texCoord);
	
	// average depth between blurred and original
	float depth = (blur.a + color.a) * 0.5;
	depth = step(depth, 0.995) * depth;
	
	// blur distance
	float mixAmount = smoothstep(0.3, 0.9, depth);
	color = mix(color, blur, mixAmount);
	
	gl_FragData[0] = color;
}

void linearizeDepth(inout float Z)
{
	Z = 2.0 * Z - 1.0;
	
	// linearize to [0, 1]
	Z = 2.0 * ZNEAR / (ZFAR + ZNEAR - Z * (ZFAR - ZNEAR));
}

#endif
