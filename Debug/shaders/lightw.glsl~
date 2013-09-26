//
// Water lighting
//

float brightness = lightdata.y * modulation;
// increase brightness if player holds an emissive item
//#include "playerlight.glsl"	

// scaled shadows
float shadow = min(1.0, min(daylight, lightdata.x) + brightness);

// shadow color
const vec3 shadowColor = vec3(-0.2, 0.0, 0.2);
color.rgb = mix(shadowColor * shadow, color.rgb, shadow);

// mix in torchlight
color.rgb = mix(color.rgb, torchlight.rgb, torchlight.a * modulation );
