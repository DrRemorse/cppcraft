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
		
		int ray_length;     // default 48
		int ray_count;      // default 1
		int seek_radius;    // default 4
		
		void init();
		
		float lightRay3D(float light, float maxlight, library::vec3 position, library::vec3 angle);
		float fastRay3D(library::vec3 position, library::vec3 angle);
		float lightRay2D(library::vec3 position, float stepx, float stepy);
		float lightRay1D(float light, float maxlight, library::vec3 const& position);
		float lightSeek(int maxRadius, int x, int y, int z);
		bool light1D(int x, int y, int z);
		vertex_color_t lightCheck(LightList& list, Sector& sector, int bx, int by, int bz, int normal, int rayCount);
		
	};
	extern LightingClass Lighting;
}

#endif
