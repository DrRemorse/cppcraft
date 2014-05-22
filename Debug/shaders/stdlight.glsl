//
// Standard lighting code snippet
//

float brightness = lightdata.y * modulation;
// increase brightness if player holds an emissive item
//#include "playerlight.glsl"

// shadow is smallest between shadow-value and daylight level
float shadow = min(1.0, min(daylight, lightdata.x) + brightness);

// corner shadows and dotlight
float cornershad = smoothstep(0.2, 0.7, lightdata.z);
cornershad = min(worldLight + brightness * 0.35, cornershad);

// scaled shadow color
//vec3 shadowColor = vec3(-0.2, 0.0, 0.2) * shadow;

// apply corner shadows & ambience
color.rgb *= cornershad * shadow;
// mix in shadows
//color.rgb = mix(shadowColor, color.rgb, shadow);
// mix in torchlight
color.rgb = mix(color.rgb, torchlight.rgb, torchlight.a * modulation);
