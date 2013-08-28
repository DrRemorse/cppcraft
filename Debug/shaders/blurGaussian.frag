#version 130
uniform sampler2D texture;
uniform vec2   sigma2;  // sigma * sigma, normalization 1/(sqrt(2*PI)*sigma)
uniform vec2   dir;     // horiz=(1.0, 0.0), vert=(0.0, 1.0)
uniform int    support; // int(sigma * 3.0) truncation

in vec2 texCoord;

void main(void)
{
	vec2 loc = texCoord;    // center pixel cooordinate
	
	vec4 acc = texture2D(texture, loc);   // accumulate center pixel
	
	for (int i = 1; i <= support; i++) {
		float coeff = exp(-0.5 * float(i) * float(i) / sigma2.x);
		acc += (texture2D(texture, loc - float(i) * dir)) * coeff; // L
		acc += (texture2D(texture, loc + float(i) * dir)) * coeff; // R
	}
	
	gl_FragColor = acc * sigma2.y; // normalize for unity gain
}
