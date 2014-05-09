#include "flatlands.hpp"

#include <library/log.hpp>
#include <library/math/toolbox.hpp>
#include <library/opengl/opengl.hpp>
#include <cstring>

using namespace library;

namespace cppcraft
{
	FlatlandsContainer flatlands;
	
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
	void FlatlandSector::reset()
	{
		// zero out flatdata structure
		memset(fdata, 0, FLATLAND_SIZE);
	}
	
	void FlatlandsContainer::init()
	{
		logger << Log::INFO << "* Initializing flatlands" << Log::ENDL;
		
		// allocate flatland sectors
		fsectors = new FlatlandSector[Sectors.getXZ() * Sectors.getXZ()];
	}
	FlatlandsContainer::~FlatlandsContainer()
	{
		delete fsectors;
	}
}
