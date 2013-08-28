//
// Final color code snippet
//
#define POSTPROCESS

#ifdef POSTPROCESS
	
	// postprocess will return color to gamma space
	gl_FragColor = vec4(color.rgb, 1.0);
	
#else
	
	const vec3 gamma = vec3(2.2);
	
	// back to gamma space
	gl_FragColor = vec4(pow(color.rgb, gamma), 1.0);
	
#endif
