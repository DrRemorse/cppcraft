/**
 * Ray-traced lighting
 * 
 * 
**/

#ifndef LIGHTING_HPP
#define LIGHTING_HPP

#include "library/math/vector.hpp"
#include "blocks.hpp"
#include "lighttable.hpp"

namespace cppcraft
{
	class Sector;
	
	class LightingClass
	{
	public:
		static const int MAX_SECTOR_REACH  = 6;
		static const int LIGHT_CULL_SECTOR = 4;
		
		static const float MAX_ADDITIONAL_LIGHT; // = 0.95f
		
		static const float DARKNESS; // = 256 * 0.90;
		static const float SHADOWS;  // = 256 * 0.68;
		static const float CORNERS;  // = 256 * 0.50;
		
		static const float RAY_CRASH_CONSTANT;  // = 16.0
		static const float LIGHT_FULL_DAMAGE;   // = 12.0
		static const float LIGHT_MEDI_DAMAGE;   // =  8.0
		
		int ray_length;     // ray length = 48
		int ray_count;      // ray count  = 2
		// crash factor is a combination of length, and screen depth
		float ray_crash_factor;
		
		void init();
		
		bool raySurvivalTest(block_t id, float& ray, float maxdmg, float& prediction, float distance_curve);
		bool damageRay(block_t id, float& ray, float maxdmg, float& prediction, float distance_curve);
		
		float lightRay3D(library::vec3 position, library::vec3 angle);
		float lightRay1D(library::vec3 position);
		vertex_color_t lightCheck(LightList& list, Sector& sector, int bx, int by, int bz);
		
	};
	extern LightingClass Lighting;
}

#endif
