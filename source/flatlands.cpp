#include "flatlands.hpp"
#include "sectors.hpp"

#include "library/log.hpp"
#include "library/math/toolbox.hpp"

using namespace library;

namespace cppcraft
{
	FlatlandsContainer Flatlands;
	
	// returns a const flatland_t struct
	FlatlandSector::flatland_t& FlatlandSector::operator() (int x, int z)
	{
		return this->fdata[x][z];
	}
	
	// returns a FlatlandSector reference from location (x, z)
	FlatlandSector& FlatlandsContainer::operator() (int x, int z)
	{
		return *this->fsectors[x * Sectors.getXZ() + z];
	}
	
	FlatlandSector::flatland_t& FlatlandsContainer::getData(int x, int z)
	{
		// find flatland sector
		int fx = x / Sector::BLOCKS_XZ;
		int fz = z / Sector::BLOCKS_XZ;
		// clamp to within local grid
		fx = toolbox::clamp(0, Sectors.getXZ()-1, fx);
		fz = toolbox::clamp(0, Sectors.getXZ()-1, fz);
		// find internal position
		int bx = x & (Sector::BLOCKS_XZ-1);
		int bz = z & (Sector::BLOCKS_XZ-1);
		// return data structure
		return this[0](fx, fz)(bx, bz);
	}
	
	int FlatlandsContainer::getTerrain(int x, int z)
	{
		return getData(x, z).terrain;
	}
	
	int FlatlandsContainer::getSkyLevel(int x, int z)
	{
		return getData(x, z).skyLevel;
	}
	
	int FlatlandsContainer::getGroundLevel(int x, int z)
	{
		return getData(x, z).groundLevel;
	}
	
	void FlatlandsContainer::init()
	{
		logger << Log::INFO << "* Initializing flatlands" << Log::ENDL;
		
		// allocate sector pointers
		fsectors = (FlatlandSector**) malloc(Sectors.getXZ() * Sectors.getXZ() * sizeof(FlatlandSector*));
		// iterate sectors
		for (int x = 0; x < Sectors.getXZ(); x++)
		{
			for (int z = 0; z < Sectors.getXZ(); z++)
			{
				// get reference to flatland pointer
				FlatlandSector*& f = manipulate(x, z);
				// set reference (FlatlandSector*) to new FlatlandSector(x, z)
				f = new FlatlandSector;
			} // z
		} // x
	}
	
}