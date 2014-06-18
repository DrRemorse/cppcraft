#ifndef SECTORS_HPP
#define SECTORS_HPP

#include "sector.hpp"

namespace cppcraft
{

	class SectorContainer
	{
	public:
		~SectorContainer();
		
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
		// returns a reference to a Sector located at (x, 0, z)
		inline Sector& operator() (int x, int z)
		{
			return *this->getSectorColumn(x, z);
		}
		
		// returns sector at position (x, y, z), or null
		Sector* sectorAt(float x, float y, float z) const;
		
		// updates all sectors, for eg. when sun-position changed
		void updateAll();
		// regenerate all sectors, for eg. teleport
		void regenerateAll();
		
	private:
		// returns a reference to a pointer to a sector, which is ONLY used by Seamless
		inline Sector*& getSectorColumn(int x, int z)
		{
			return this->sectors[x * sectors_XZ + z];
		}
		inline Sector* getSectorPtr(int x, int y, int z) const
		{
			// custom order: (x, z, y)
			// internal only, outside sources don't care
			return this->sectors[x * sectors_XZ + z] + y;
		}
		
		// Seamless: moves sector (x2, z2) to (x, z)
		inline void move(int x, int z, int x2, int z2)
		{
			Sector*& s = getSectorColumn(x, z);
			// move to new position
			s = getSectorColumn(x2, z2);
			s->x = x;
			s->z = z;
		}
		
		Sector** sectors;
		int sectors_XZ; // sectors XZ-axes size
		
		friend class Seamless;
		friend class Spiders;
	};
	extern SectorContainer Sectors;
	
}

#endif
