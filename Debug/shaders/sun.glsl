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

void main(void)
{
	texCoord = in_vertex.xy + vec2(0.5);;
	
	vec4 position = matview * vec4(in_vertex.xyz, 1.0);
	
	norm = normalize(position.xyz * mat3(matrot));
	
	gl_Position = matproj * position;
}

#endif

#ifdef FRAGMENT_PROGRAM
uniform sampler2D texture;

in vec2 texCoord;
in vec3 norm;

void main(void)
{
	float horizon = clamp(-norm.y / 0.025,  0.0, 1.0);
	
	vec3 color = texture2D(texture, texCoord).rgb;
	float alpha = smoothstep(0.0, 0.9, color.r);
	
	color.rgb = mix(vec3(1.0, 0.8, 0.6), vec3(1.0), pow(alpha, 8.0));
	
	// limit sun to horizon
	alpha *= pow(1.0 - horizon, 3.0);
	
	//if (alpha < 0.1) discard;
	
	gl_FragColor = vec4(color, alpha);
}

#endif
