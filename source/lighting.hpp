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
		
		static const float DARKNESS;
		static const float SHADOWS;
		static const float AMB_OCC;
		static const float CORNERS;
		
		int ray_length;     // ray length = 48
		int ray_count;      // ray count  = 2
		
		void init();
		
		float lightRay3D(float slight, float darkness, library::vec3 position, library::vec3 angle);
		float lightRay2D(float slight, float darkness, library::vec3 position, float stepx, float stepy);
		float lightRay1D(float slight, float darkness, library::vec3 position);
		vertex_color_t lightCheck(LightList& list, Sector& sector, int bx, int by, int bz, int rayCount);
		
	};
	extern LightingClass Lighting;
}

#endif
