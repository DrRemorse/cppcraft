
	worldLight = 0.85 + 0.2 * daylight * dot(in_normal.xy, lightVector.xy);
	
	//worldLight += 0.025 * in_normal.y;   // if (y != 0) d += 0.025
