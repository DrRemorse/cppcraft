#version 130
#extension GL_EXT_gpu_shader4 : enable
uniform sampler2DArray texture;
uniform vec3 focal;
uniform vec4 playerLight;
uniform float daylight;
uniform float modulation;

in vec3 texCoord;
in vec4 colordata;
in float vertdist;
in float worldLight;

const vec3 degamma = vec3(1.0 / 2.2);
const vec3 gamma   = vec3(2.2);

void main(void) {
	
	if (vertdist >= focal[2]) { discard; }
	
	highp vec4 color = texture2DArray(texture, texCoord);
	if (color.a < 0.1) { discard; } // le discard!
	
	// degamma
	color.rgb = pow(color.rgb, degamma);
	
	float brightness = 0.0f;
	
	// playerlight
	if (vertdist < playerLight[0]) {
		float ldist = playerLight[0] - vertdist;
		float str = pow(ldist * ldist / playerLight[1], playerLight[2]);
		
		brightness = min(1.0, brightness + str * playerLight[3] * modulation);
	}
	
	//color.rgb *= min(1.0, worldLight * daylight + brightness);
	color.rgb = mix(color.rgb, colordata.rgb, max(0.0, colordata.a - brightness));
	
	// gamma ramp
	gl_FragColor = vec4(pow(color.rgb, gamma), 1.0);
	
}
