//
// Horizon fade code snippet
//
#define POSTPROCESS

const float fade_dist = 0.85 * ZFAR;

#ifdef POSTPROCESS
	
	color.a = 1.0 - clamp((vertdist - fade_dist) / (ZFAR - fade_dist), 0.0, 1.0);
	
#else
	
	if (vertdist > fade_dist)
	{
		float fade = (vertdist - fade_dist) / (ZFAR * (1.0 - fade_dist));
		
		vec3 skyColor = texture2D(skybuffer, gl_FragCoord.xy / screendata.xy).rgb;
		skyColor = pow(skyColor, vec3(1.0 / 2.2));
		
		color.rgb = mix(color.rgb, skyColor, fade * fade);
	}
	
#endif
