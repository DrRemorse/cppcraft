#include "sectors.hpp"

#include "generator.hpp"

namespace cppcraft
{
	SectorContainer Sectors;
	
	Sector* SectorContainer::sectorAt(float x, float y, float z) const
	{
		if (y >= 0 && y < Sector::BLOCKS_Y  * Sectors.getY() &&
			x >= 0 && x < Sector::BLOCKS_XZ * Sectors.getXZ() &&
			z >= 0 && z < Sector::BLOCKS_XZ * Sectors.getXZ())
		{
			return getSectorPtr(x / Sector::BLOCKS_XZ, y / Sector::BLOCKS_Y, z / Sector::BLOCKS_XZ);
		}
		return nullptr;
	}
	
	// allocates and initializes all Sectors
	void SectorContainer::init(int sectors_xz)
	{
		// set number of sectors on X/Z axes
		this->sectors_XZ = sectors_xz;
		// allocate sector pointers
		sectors = new Sector*[sectors_XZ * sectors_XZ];
		// iterate sectors
		for (int x = 0; x < sectors_XZ; x++)
		for (int z = 0; z < sectors_XZ; z++)
		{
			Sector* &first = getSectorColumn(x, z);
			first = new Sector[SECTORS_Y];
			
			for (int y = 0; y < SECTORS_Y;  y++)
			{
				// initialize sector
				first[y] = Sector(x, y, z);
			}
			
		} // y, z, x
	}
	SectorContainer::~SectorContainer()
	{
		// iterate sectors
		for (int x = 0; x < sectors_XZ; x++)
		for (int z = 0; z < sectors_XZ; z++)
		{
			delete[] getSectorColumn(x, z);
		}
		delete[] this->sectors;
	}
	
	void SectorContainer::invalidateAll()
	{
		// iterate sectors
		for (int x = 0; x < sectors_XZ; x++)
		for (int z = 0; z < sectors_XZ; z++)
		for (int y = 0; y < SECTORS_Y;  y++)
		{
			// get reference to Sector pointer
			Sector &s = Sectors(x, y, z);
			// recompile sector, if (we know) its already renderable
			if (s.render) s.progress = Sector::PROG_NEEDRECOMP;
		} // y, z, x
	}
}
