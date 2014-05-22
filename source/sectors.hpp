#ifndef SECTORS_HPP
#define SECTORS_HPP

#include "sector.hpp"

namespace cppcraft
{

	class SectorContainer
	{
	private:
		Sector** sectors;
		int sectors_XZ; // sectors XZ-axes size
		
	public:
		static const int SECTORS_Y = 32;
		static const int SECTORS_Y_SHL = 5;
		static const int MAX_SECTORS_XZ_GRIDSIZE = 128;
		
		// initializes with axis length, called from World::init()
		void init(int xz);
		
		inline int getXZ() const { return this->sectors_XZ; }
		inline int getY()  const { return SECTORS_Y; }
		
		// returns a reference to a Sector located at (x, y, z)
		inline Sector& operator() (int x, int y, int z)
		{
			return *this->getSectorPtr(x, y, z);
		}
		
		// returns sector at position (x, y, z), or null
		Sector* sectorAt(float x, float y, float z) const;
		
		// invalidates all sectors, eg. when sun-position permanently changed
		void invalidateAll();
		
	private:
		// returns a reference to a pointer to a sector, which is ONLY used by Seamless
		inline Sector* & getSectorPtr(int x, int y, int z) const
		{
			// custom order: (x, z, y)
			// internal only, outside sources don't care
			return this->sectors[((x * sectors_XZ + z) << SECTORS_Y_SHL) + y];
		}
		
		// moved Sector (x2, y2, z2) to (x, y, z)
		inline void set(int x, int y, int z, int x2, int y2, int z2)
		{
			Sector*& s = getSectorPtr(x, y, z);
			// set to new position
			s = getSectorPtr(x2, y2, z2);
			// assumption: not swapping in height
			s->x = x;
			s->z = z;
		}
		
		friend class Seamless;
		friend class Spiders;
	};
	extern SectorContainer Sectors;
	
}

#endif
