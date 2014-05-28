//
// Final color code snippet
//
#define POSTPROCESS

#ifndef POSTPROCESS
	
	const vec3 gamma = vec3(2.2);
	
	// back to gamma space
	color = vec4(pow(color.rgb, gamma), color.a);
	
#endif
