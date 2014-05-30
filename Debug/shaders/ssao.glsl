float ambientOcclusion(in vec2 coord, in vec3 pos, in vec3 normal)
{
	const float g_scale = 0.25;
	const float g_bias  = 0.0;
	const float g_intensity = 0.8;
	
	vec3 samplePos = getPosition(coord);
	vec3 diff = samplePos - pos;
	
	float d = length(diff);
	vec3  v = diff / d;
	
	vec3 sampleNorm = getNormal(coord);
	float dotp = dot(normal, v);
	
	dotp = 1.0 - dot(normal, sampleNorm);
	
	d = 1.0 / (1.0 + d * g_scale);
	return max(0.0, dotp - g_bias) * d * g_intensity;
}

float getAO(vec2 uv)
{
	vec3 p = getPosition(uv);
	vec3 n = getNormal(uv);
	vec2 rand = texture(randomtex, texCoord * 8.0).rg * 1.0 - 0.5;
	
	const float sample_radius = 0.2;
	const int iterations = 4;
	
	const vec2 offsetsAO[4] = vec2[]
	(
		vec2(1.0, 0.0), vec2(-1.0, 0.0),
		vec2(0.0, 1.0), vec2( 0.0,-1.0)
	);
	
	float ao  = 0.0;
	float rad = sample_radius / p.z;
	
	//** SSAO Calculation **//
	for (int j = 0; j < iterations; ++j)
	{
		vec2 coord1 = reflect(offsetsAO[j], rand) * rad;
		vec2 coord2 = vec2(coord1.x*0.707 - coord1.y*0.707,
						   coord1.x*0.707 + coord1.y*0.707);
	  
	  ao += ambientOcclusion(uv + coord1*0.25, p, n);
	  ao += ambientOcclusion(uv + coord2*0.50, p, n);
	  ao += ambientOcclusion(uv + coord1*0.75, p, n);
	  ao += ambientOcclusion(uv + coord2, p, n);
	}
	return ao / (float(iterations) * 4.0);
}
