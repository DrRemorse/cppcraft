#ifndef SUN_HPP
#define SUN_HPP

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
		
		// permanently sets the static sun angle
		void setRadianAngle(float radians);
		
		// getters for static sun values
		const library::vec3& getAngle() const;
		const library::vec3& getHalfAngle() const;
		const library::vec3& getHalf2Angle() const;
		float getRadianAngle() const;
		
		// daylight multiplier: 1.0 = full daylight, 0.0 = total darkness
		float daylight(float y);
		// world ambient light color
		library::vec3 ambienceColor();
		
		// integrator for realtime sun values
		void integrate(float step);
		// travel distance check for updating shadows
		void travelCheck();
		// gettesr for realtime sun values
		const library::vec3& getRealtimeAngle() const;
		float getRealtimeRadianAngle() const;
		float getRealtimeDaylight() const;
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
		float realAmbience;
		
		float renderDist;
		
		// sky renderer uses this class extensively
		friend class SkyRenderer;
		friend class Textureman;
		friend class SceneRenderer;
	};
	extern SunClass thesun;
	
}

#endif
