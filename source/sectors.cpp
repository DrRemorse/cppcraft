#include "sectors.hpp"

#include "generator.hpp"
#include "generatorq.hpp"

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
	
	void SectorContainer::updateAll()
	{
		// iterate sectors
		for (int x = 0; x < sectors_XZ; x++)
		for (int z = 0; z < sectors_XZ; z++)
		{
			// get column
			Sector* base = getSectorColumn(x, z);
			// update all sectors in column
			for (int y = 0; y < SECTORS_Y;  y++)
			{
				// recompile sector, if (we know) its already renderable
				if (base[y].render) base[y].progress = Sector::PROG_NEEDRECOMP;
			}
		} // y, z, x
	}
	void SectorContainer::regenerateAll()
	{
		// iterate sectors
		for (int x = 0; x < sectors_XZ; x++)
		for (int z = 0; z < sectors_XZ; z++)
		{
			// get column
			Sector* base = getSectorColumn(x, z);
			// clear all sectors in column, and add to generator queue
			for (int y = 0; y < SECTORS_Y;  y++)
			{
				base[y].invalidate();
			}
			Generator::generate(Sectors(x, 0, z), nullptr, 0.0);
		} // y, z, x
	}
}
