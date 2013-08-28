//
// Final color code snippet
//
#define POSTPROCESS

#ifdef POSTPROCESS
	
	// postprocess will return color to gamma space
	gl_FragData[0] = color;
	
#else
	
	const vec3 gamma = vec3(2.2);
	
	// back to gamma space
	gl_FragData[0] = vec4(pow(color.rgb, gamma), color.a);
	
#endif
