#ifndef PRECOMP_THREAD_AO_HPP
#define PRECOMP_THREAD_AO_HPP

#include <library/storage/bitarray.hpp>
#include "sector.hpp"
#include <cstring>

namespace cppcraft
{
	class AmbientOcclusion
	{
	public:
		static const int CRASH_MAX_POINTS = 2400;
		static const int POINT_ELEMENTS = CRASH_MAX_POINTS * 8 * 6;
		
		AmbientOcclusion()
		{
			pointData = library::BitArray(POINT_ELEMENTS);
		}
		
		// get/set point bits
		inline void setPoint(int index, int corner, int face)
		{
			pointData.set((index * 8 + corner) * 6 + face);
		}
		inline bool getPoint(int index, int corner, int face) const
		{
			return pointData[(index * 8 + corner) * 6 + face];
		}
		
		void clear()
		{
			count = 0;
			// clear point bitarray
			pointData.clear();
			// clear lookup table
			memset(t_lookup, 0, sizeof(t_lookup));
		}
		
		// number of corner points set (must be cleared)
		int count;
		
		// lookup table (must be cleared)
		short t_lookup[Sector::BLOCKS_XZ+1]
					  [Sector::BLOCKS_Y +1]
					  [Sector::BLOCKS_XZ+1];
		// corner shadow value (does not need to be cleared)
		unsigned char t_corner[CRASH_MAX_POINTS];
		// test data (does not need to be cleared)
		visiblefaces_t testdata;
	private:
		// bit array for defining which faces are connected to each point (must be cleared)
		library::BitArray pointData;
	};
	
}

#endif
