
	float worldLight = 0.2 * daylight;
	if (gl_FrontFacing || texrange == TX_CROSS)
		worldLight = 0.85 + worldLight * dot(out_normal.xy, lightVector.xy);
	else
		worldLight = 0.85 + worldLight * dot(vec2(-out_normal.x, out_normal.y), lightVector.xy);
	
	//worldLight += 0.025 * out_normal.y;   // if (y != 0) d += 0.025
