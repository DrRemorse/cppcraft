vec3 pSphere[16] = vec3[]
(
	vec3(0.53812504, 0.18565957, -0.43192),
	vec3(0.13790712, 0.24864247, 0.44301823),
	vec3(0.33715037, 0.56794053, -0.005789503),
	vec3(-0.6999805, -0.04511441, -0.0019965635),
	vec3(0.06896307, -0.15983082, -0.85477847),
	vec3(0.056099437, 0.006954967, -0.1843352),
	vec3(-0.014653638, 0.14027752, 0.0762037),
	vec3(0.010019933, -0.1924225, -0.034443386),
	vec3(-0.35775623, -0.5301969, -0.43581226),
	vec3(-0.3169221, 0.106360726, 0.015860917),
	vec3(0.010350345, -0.58698344, 0.0046293875),
	vec3(-0.08972908, -0.49408212, 0.3287904),
	vec3(0.7119986, -0.0154690035, -0.09183723),
	vec3(-0.053382345, 0.059675813, -0.5411899),
	vec3(0.035267662, -0.063188605, 0.54602677),
	vec3(-0.47761092, 0.2847911, -0.0271716)
);

#define SAMPLES 16

float getSSAO(float depth)
{
	const float totStrength = 1.38;
	const float strength = 0.07;
	const float offset = 18.0;
	const float falloff = 0.000002;
	const float rad = 0.006;
	
	vec3 noise  = normalize(texture2D(noisetex, texCoord * offset).xyz * 2.0 - vec3(1.0));
	vec3 normal = texture2D(normals, texCoord).xyz * 2.0 - vec3(1.0);
	
	vec3 ep = vec3(texCoord, depth);
	
	float radD = rad / depth;
	
	vec3 ray;
	vec3 se;
	float AO = 0.0;
	
	for (int i = 0; i < SAMPLES; i++)
	{
		ray = radD * reflect(pSphere[i], noise);
		
		// if the ray is outside the hemisphere then change direction
		se = ep + sign(dot(ray, normal)) * ray;
		
		// get the depth of the occluder fragment
		vec4 occluder = texture2D(normals, se.xy);
		occluder.xyz = occluder.xyz * 2.0 - vec3(1.0);
		
		// if depthDifference is negative = occluder is behind current fragment
		float depthDifference = depth - occluder.a;
		float normDiff = 1.0 - dot(occluder.xyz, normal.xyz);
		
		// the falloff equation, starts at falloff and is kind of 1/x^2 falling
		AO += step(falloff, depthDifference) * normDiff * (1.0 - smoothstep(falloff, strength, depthDifference));
		
	}
	
	return 1.0 - totStrength * AO * (1.0 / SAMPLES);
}
