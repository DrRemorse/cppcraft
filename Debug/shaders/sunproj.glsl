#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

//precision mediump float;

#ifdef VERTEX_PROGRAM
uniform mat4 matproj;
uniform mat4 matview;
uniform mat4 matrot;

in vec3 in_vertex;

out vec2 pos;
out vec3 norm;
out vec2 screenPos;

void main(void)
{
	pos = in_vertex.xy;
	
	vec4 position = matview * vec4(in_vertex.xyz, 1.0);
	
	norm = normalize(position.xyz * mat3(matrot));
	
	gl_Position = matproj * position;
	// scale position to range [0, 1] on screen
	screenPos = gl_Position.xy / gl_Position.w / vec2(2.0) + vec2(0.5);
}

#endif

#ifdef FRAGMENT_PROGRAM
uniform sampler2D texture;

uniform vec3  screendata;

in vec2 pos;
in vec3 norm;
in vec2 screenPos;

const vec3 SUNCOLOR = vec3(0.9, 0.7, 0.5);

const float ZFAR
const float ZNEAR

float getDepth(vec2 coord)
{
	float depth = texture2D(texture, coord).r;
	
	// linearize
	return (2.0 * ZNEAR) / (ZFAR + ZNEAR - depth * (ZFAR - ZNEAR));
}

void main(void)
{
	// distance from center
	float rad = (0.5 - length(pos)) * 2.0;
	if (rad <= 0.0) discard;
	
	// something is blocking
	if ( texture2D(texture, screenPos).r < 1.0 ) discard;
	
	float rad2 = rad * rad;
	
	vec3 color = SUNCOLOR * ( 0.5 * rad + 1.0 * rad2 );
	
	float cake = rad;
	cake = min(1.0, pow(cake, 3.0));
	
	// limit flare to horizon
	cake *= pow( 1.0 - clamp(-norm.y / 0.001,  0.0, 1.0),  3.0 );
	
	gl_FragData[0] = vec4(color * cake, 1.0);
	gl_FragDepth = 1.0;
}
#endif
