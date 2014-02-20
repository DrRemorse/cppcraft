
	// overall lighting on each face based on normal
	worldLight = 0.85 + 0.2 * daylight * dot(in_normal.xy, lightVector.xy);
