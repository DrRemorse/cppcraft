//
// Horizon fade code snippet
//
#define POSTPROCESS
const float fade_dist = ZFAR * 0.85;

if (vertdist > fade_dist)
{
	float fade_ = min(1.0, (vertdist - fade_dist) / (ZFAR - fade_dist));
	
	vec3 skyColor = texture2D(skybuffer, gl_FragCoord.xy / screendata.xy).rgb;
#ifndef POSTPROCESS
	skyColor = pow(skyColor, vec3(1.0 / 2.2));
#endif
	color.rgb = mix(color.rgb, skyColor, fade_ * fade_);
}
