#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM
precision mediump float;

#ifdef VERTEX_PROGRAM
uniform mat4 matproj;
uniform mat4 matview;
uniform mat4 matrot;

in vec3 in_vertex;

out vec2 texCoord;
out vec3 norm;
out vec2 screenPos;

void main(void)
{
	texCoord = in_vertex.xy + vec2(0.5);
	
	vec4 position = matview * vec4(in_vertex.xyz, 1.0);
	
	norm = normalize(position.xyz * mat3(matrot));
	
	gl_Position = matproj * position;
	// scale position to range [0, 1] on screen
	screenPos = gl_Position.xy / gl_Position.w / vec2(2.0) + vec2(0.5);
}

#endif

#ifdef FRAGMENT_PROGRAM
uniform sampler2D texture;
uniform sampler2D depth;

in vec2 texCoord;
in vec3 norm;
in vec2 screenPos;

const vec3 SUNCOLOR = vec3(0.9, 0.7, 0.5);

const float ZFAR
const float ZNEAR

void main(void)
{
	vec3 color = texture2D(texture, texCoord).rgb;
	color *= vec3(0.9, 0.7, 0.5) * 0.6;
	
	// discard if something is blocking
	color.rgb *= step(0.995, texture2D(depth, screenPos).a);
	
	// limit flare to horizon
	color *= pow( 1.0 - clamp(-norm.y / 0.01,  0.0, 1.0),  3.0 );
	
	gl_FragData[0] = vec4(color, 1.0);
	gl_FragDepth = 1.0;
}
#endif
