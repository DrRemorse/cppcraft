#include "sun.hpp"

#include "library/math/matrix.hpp"
#include "camera.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	const double PI = 4 * atan(1);
	const double PI2 = PI * 2;
	const float SunClass::SUN_DEF_ANGLE = PI / 4.0;
	SunClass thesun;
	
	void SunClass::init(float angle)
	{
		this->radianAngle = angle;
		this->setRadianAngle(this->radianAngle);
		this->renderDist = 4.0;
		
		this->realRadian   = angle + PI;
		this->realAmbience = daylight(realRadian);
	}
	
	const vec3& SunClass::getAngle() const
	{
		return this->angle;
	}
	const vec3& SunClass::getHalfAngle() const
	{
		return this->half1;
	}
	const vec3& SunClass::getHalf2Angle() const
	{
		return this->half2;
	}
	
	void SunClass::setRadianAngle(float angle)
	{
		// angle fmod= PI2
		while (angle >= PI2) angle -= PI2;
		while (angle <  0.0) angle += PI2;
		
		this->radianAngle = angle;
		this->angle.x   = cos(angle);
		this->angle.y   = sin(angle);
		
		float direction = 1.0;
		if (this->angle.x < 0) direction = -1.0;
		
		half1 = vec3(this->angle.x + direction * 0.08, this->angle.y, 0.0);
		half1.normalize();
		half2 = vec3(this->angle.x + direction * 0.12, this->angle.y, 0.0);
		half2.normalize();
	}
	
	void SunClass::integrate(float step)
	{
		vec3 newAngle = getRealtimeAngle().mix(getAngle(), step);
		
		realRadian = atan2(newAngle.y, newAngle.x);
		
		realAngle = vec3(cos(realRadian), sin(realRadian), 0.0);
		realAmbience = 1.0 - daylight(realAngle.y);
	}
	
	float SunClass::getRadianAngle() const
	{
		return this->radianAngle;
	}
	
	// returns the current light level
	float SunClass::daylight(float y)
	{
		const float transitional_light =  0.0;
		const float transitional_dark  = -0.2;
		
		const float noonlight = 0.2;
		const float nightlight = 0.75;
		const float moonlight  = 0.85;
		
		float transit = 0.0;
		
		if (y >= transitional_light)
		{
			transit = 1.0 - (y - transitional_light) / (1.0 - transitional_light);
			return noonlight * powf(transit, 3.0);
		}
		else if (y < transitional_dark)
		{
			transit = 1.0 - (fabs(y) - transitional_dark) / (1.0 - transitional_dark);
			return moonlight * (1.0 - transit) + nightlight * transit;
		}
		
		// inside transitional phase
		float translength = fabs(transitional_light - transitional_dark);
		
		transit = 1.0 - (y + fabs(transitional_dark)) / translength;
		
		return noonlight + (nightlight - noonlight) * transit;
	}
	
	vec3 SunClass::ambienceColor()
	{
		float yy = fabs(angle.y) * angle.y; // ogl.lastsun(1)) * ogl.lastsun(1);
		
		if (yy < -0.15)
		{
			return vec3(1.0 + yy, 1.0 + yy,  1.0);
		}
		else
		{
			float redness = 0.7 + 0.3 * yy;
			#define blueness 0
			
			return vec3(1.0 - blueness, 0.4 + redness * 0.6 - blueness, redness);
		}
	}
	
	const vec3& SunClass::getRealtimeAngle() const
	{
		return realAngle;
	}
	float SunClass::getRealtimeRadianAngle() const
	{
		return realRadian;
	}
	float SunClass::getRealtimeDaylight() const
	{
		return realAmbience;
	}
	
	mat4 SunClass::getSunMatrix() const
	{
		mat4 mattemp = rotationMatrix(0.0, -PI / 2, 0.0);
		mat4 matsun = rotationMatrix(thesun.getRealtimeRadianAngle(), 0.0, 0.0);
		mattemp *= matsun;
		mattemp.translate(0.0, 0.0, -thesun.renderDist);
		
		return camera.getRotationMatrix() * mattemp;
	}
	
}
