#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

precision mediump float;

#ifdef VERTEX_PROGRAM
uniform mat4 matproj;
uniform mat4 matview;
uniform mat4 matrot;

in vec3 in_vertex;

out vec2 pos;
out vec3 norm;

void main(void)
{
	pos = in_vertex.xy;
	
	vec4 position = matview * vec4(in_vertex.xyz, 1.0);
	
	norm = normalize(position.xyz * mat3(matrot));
	
	gl_Position = matproj * position;
}

#endif

#ifdef FRAGMENT_PROGRAM

in vec2 pos;
in vec3 norm;

const vec3 SUNCOLOR = vec3(0.9, 0.7, 0.5);

void main(void)
{
	// limit moon to horizon
	float horizon = clamp(-norm.y / 0.025,  0.0, 1.0);
	
	// distance from center
	float rad = max(0.0, 0.5 - length(pos)) * 2.0;
	float rad2 = rad * rad;
	
	//const vec2 center = normalize(vec2(0.5));
	//float angle = acos(dot(center, normalize(pos)));
	//float wave = 1.0 + 0.08 * cos(angle * 30 - frameRadian[2] * 0.05) * cos(angle * 12 + frameRadian[2] * 0.025);
	
	//float cake = 0.5 * rad2 + rad * wave;
	//vec3 color = SUNCOLOR * ( 0.75 + rad2 + 0.5 * rad2 * rad2 );
	
	vec3 color = SUNCOLOR * ( 0.8 + 0.25 * rad + 1.0 * rad2 );
	
	float cake = 0.75 * sqrt(rad) + 0.75 * rad2;
	cake = min(1.0, pow(cake, 4.0));
	
	// limit sun to horizon
	cake *= pow(1.0 - horizon,  4.0);
	
	gl_FragColor = vec4(color, cake);
}

#endif
