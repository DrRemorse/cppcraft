
// pixel-based dot-light
float worldLight = 0.2 * daylight;
if (gl_FrontFacing) // || texrange == TX_CROSS)
	worldLight *= dot(out_normal.xyz, lightVector.xyz);
else
	worldLight *= dot(vec3(-out_normal.x, out_normal.yz), lightVector.xyz);
worldLight += 0.85;
