#ifndef THESUN_HPP
#define THESUN_HPP

#include <library/math/vector.hpp>

namespace library
{
	class mat4;
}

namespace cppcraft
{
	class SunClass
	{
	public:
		static const float SUN_DEF_ANGLE;
		
		void init(float radangle);
		void render();
		
		// permanently sets new static sun angle
		void setRadianAngle(float radians);
		// getter for static sun angle
		float getRadianAngle() const
		{
			return this->radianAngle;
		}
		
		// getters for motionless sun
		const library::vec3& getAngle() const
		{
			return this->angle;
		}
		const library::vec3& getHalfAngle() const
		{
			return this->half1;
		}
		const library::vec3& getHalf2Angle() const
		{
			return this->half2;
		}
		
		// daylight multiplier: 1.0 = full daylight, 0.0 = total darkness
		float daylight(float y);
		// world ambient light color
		library::vec3 ambienceColor();
		
		// integrator for realtime sun values
		void integrate(float step);
		// travel distance check for updating shadows
		void travelCheck();
		// gettesr for realtime sun values
		const library::vec3& getRealtimeAngle() const
		{
			return realAngle;
		}
		float getRealtimeRadianAngle() const
		{
			return realRadian;
		}
		float getRealtimeDaylight() const
		{
			return realAmbience;
		}
		
		// each frame set realtime view-sunvector
		void setRealtimeSunView(const library::mat4& matrot);
		// getter for realtime view-sunvector
		const library::vec3& getRealtimeViewAngle() const
		{
			return realViewAngle;
		}
		
		// timestep
		void setStep(int step);
		float getStepValue();
		
		library::mat4 getSunMatrix() const;
		
	private:
		float step;
		float radianAngle;   // target angle
		library::vec3 angle; // target sun vector from player
		library::vec3 half1;
		library::vec3 half2;
		
		float realRadian;
		library::vec3 realAngle;
		library::vec3 realViewAngle;
		float realAmbience;
		
		float renderDist;
	};
	extern SunClass thesun;
	
}

#endif
