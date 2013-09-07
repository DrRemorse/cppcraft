#version 130
#define VERTEX_PROGRAM
#define FRAGMENT_PROGRAM

#ifdef VERTEX_PROGRAM

uniform mat4 matproj;
uniform mat4 matview;

uniform vec3 v3CameraPos;		// The camera's current position
uniform vec3 v3LightPos;		// The direction vector to the light source
uniform vec3 v3InvWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
uniform float fCameraHeight;	// The camera's current height
uniform float fOuterRadius;		// The outer (atmosphere) radius
uniform float fInnerRadius;		// The inner (planetary) radius
uniform float fKrESun;			// Kr * ESun
uniform float fKmESun;			// Km * ESun
uniform float fKr4PI;			// Kr * 4 * PI
uniform float fKm4PI;			// Km * 4 * PI
uniform float fScale;			// 1 / (fOuterRadius - fInnerRadius)
uniform float fScaleDepth;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
uniform float fScaleOverScaleDepth;	// fScale / fScaleDepth
uniform float above; // 1.0 above horizon, 0.0 below horizon

#define nSamples 4
#define fSamples 4.0

in vec3 in_vertex;

out vec3 v3Direction;
out vec3 color_rayleigh;
out vec3 color_mie;
out vec3 starc;

float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

void main(void)
{
	// Get the ray from the camera to the vertex, and its length (which is the far point of the ray passing through the atmosphere)
	vec3 v3Ray = in_vertex.xyz - v3CameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;
	
	// Calculate the ray's starting position, then calculate its scattering offset
	vec3 v3Start = v3CameraPos;
	float fHeight = length(v3Start);
	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
	float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	float fStartOffset = fDepth * scale(fStartAngle);
	
	// Initialize the scattering loop variables
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;
	
	// Now loop through the sample rays
	vec3 v3FrontColor = vec3(0.0);
	for(int i = 0; i < nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fLightAngle = dot(v3LightPos, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
		vec3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}
	
	starc = normalize(v3Ray);
	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
	color_mie = v3FrontColor * fKmESun; // * above;
	color_rayleigh = v3FrontColor * (v3InvWavelength * fKrESun);
	v3Direction = v3CameraPos - in_vertex.xyz;
	
	gl_Position = matproj * matview * vec4(in_vertex.xyz, 1.0);
}

#endif

#ifdef FRAGMENT_PROGRAM
#define POSTPROCESS

uniform samplerCube texture;
uniform sampler2D   stars;

uniform float above;
uniform float sunAngle;

uniform vec3 v3LightPos;
uniform float g;
uniform float g2;

in vec3 v3Direction;
in vec3 color_rayleigh;
in vec3 color_mie;
in vec3 starc;

const float exposure = 2.0;

void main (void)
{
	float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);
	float fRayleighPhase = 0.75 * (1.0 + fCos*fCos);
	float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
	
	vec3 final = fRayleighPhase * color_rayleigh + fMiePhase * color_mie;
	
	vec3 norm = normalize(vec3(v3Direction.x, -v3Direction.y, v3Direction.z));
	if (above < 0.5) norm.y = -norm.y;
	
	vec3 skymap = textureCube(texture, norm).rgb;
	final = mix(final, skymap, final.b * 1.1 - final.r * 0.5 );
	
	float darkness = max(0.0, 0.16 - length(final)) / 0.16 * 0.707;
	if (darkness > 0.05)
	{
		// stars
		const float PI = 3.1415926;
		vec2 coord = vec2(((atan(norm.y, norm.x) + sunAngle) / PI + 1.0) * 0.5, asin(norm.z) / PI + 0.5 );
		
		vec3 stars = pow(texture2D(stars, coord).rgb, vec3(2.5));
		
		final = mix(final, stars, darkness * darkness);
	}
	
	//final = pow(final, vec3(1.0 / 2.2)) * 1.05;
	final = vec3(1.0) - exp(final * -exposure);
	gl_FragData[0] = vec4(final, 1.0);
}

#endif
