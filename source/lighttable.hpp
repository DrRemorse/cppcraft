#ifndef LIGHTTABLE_HPP
#define LIGHTTABLE_HPP

#include <vector>

namespace cppcraft
{
	typedef unsigned long long vertex_color_t;
	class Sector;
	
	class PrecompScratchTable
	{
		// NOTE: bounds from (-1, -1, -1) to (BLOCKS_X, BLOCKS_Y, BLOCKS_Z) inclusive
		
		int elements;
		// bool for setting "slot is used" flag
		bool           * inuse;
		// complex color, containing shadow + fake AO + emissive color (8 channels)
		vertex_color_t * value;
		
	public:
		PrecompScratchTable();
		
		void clear();
		inline int element(int, int, int) const;
		void set(int, int, int, vertex_color_t);
		bool           isset(int, int, int) const;
		vertex_color_t color(int, int, int) const;
		
	}; // ltable2
	
	struct blocklight_t
	{
		// light location, used in calculating distance
		Sector* s;
		short bx, by, bz;
		// light id (referring to global light list)
		short id;
	};
	
	class LightList
	{
	public:
		std::vector<blocklight_t> lights;
		PrecompScratchTable table;
		// true if lights has been gathered into the lights list
		// is set to false when the precompiler starts
		vertex_color_t tableLight(Sector& sector, int x, int y, int z);
		
		bool gathered;
	};
}

#endif
