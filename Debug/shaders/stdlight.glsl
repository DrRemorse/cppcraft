//
// Standard lighting code snippet
//

#define brightness  lightdata.y // * modulation;

// shadow is smallest between shadow-value and daylight level
lowp float shadow = min(1.0, daylight * lightdata.x + brightness);

// corner shadows and dotlight
lowp float cornershad = smoothstep(0.0, 1.0, lightdata.z);
cornershad = min(1.0, cornershad * worldLight + brightness * 0.125);

// scaled shadow color
//vec3 shadowColor = vec3(-0.2, 0.0, 0.2) * shadow;

// apply corner shadows & ambience
color.rgb *= cornershad * shadow;
// mix in shadows
//color.rgb = mix(shadowColor, color.rgb, shadow);
// mix in torchlight
color.rgb = mix(color.rgb, torchlight.rgb, torchlight.a * modulation);
