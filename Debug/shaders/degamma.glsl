//
// degamma
//
//#define POSTPROCESS

//#ifdef POSTPROCESS
	
	const vec3 degamma = vec3(1.0 / 2.2);
	
	// degamma
	color.rgb = pow(color.rgb, degamma);
	
//#endif
