//
// Water lighting
//

float brightness = lightdata.y * modulation;

// scaled shadows
float shadow = min(1.0, lightdata.x + brightness);

// shadow color
//const vec3 shadowColor = vec3(-0.2, 0.0, 0.2);
color.rgb *= shadow; //mix(shadowColor * shadow, color.rgb, shadow);

// mix in torchlight
color.rgb = mix(color.rgb, torchlight.rgb, torchlight.a * modulation);
