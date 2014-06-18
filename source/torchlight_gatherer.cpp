#include "torchlight.hpp"

#include <library/log.hpp>
#include "sectors.hpp"

using namespace library;

namespace cppcraft
{
	// lights top-face test
	bool LightidityCompTop(block_t id)
	{
		return isAir(id) || (id > LOWBLOCK_END && id != _LAVABLOCK);
	}
	// lights visibility test
	bool LightidityComp(block_t id)
	{
		return isAir(id) || (id >= HALFBLOCK_START && id != _LAVABLOCK);
	}
	
	bool lightGathererHelper(Sector& sector, int bx, int by, int bz)
	{
		if (bx == 0) // facing = 5
		{
			if (sector.getX() > 0)
			{
				Sector& s = Sectors(sector.getX()-1, sector.getY(), sector.getZ());
				if (s.contents > Sector::CONT_UNKNOWN)
				{
					if (LightidityComp(s(Sector::BLOCKS_XZ-1, by, bz).getID()))
						return true;
				}
				else return true;
			}
		}
		else
		{
			if (LightidityComp(sector(bx-1, by, bz).getID()))
				return true;
		}
		
		if (bx == Sector::BLOCKS_XZ-1) // facing = 4
		{
			if (sector.getX() < Sectors.getXZ()-1)
			{
				Sector& s = Sectors(sector.getX()+1, sector.getY(), sector.getZ());
				if (s.contents > Sector::CONT_UNKNOWN)
				{
					if (LightidityComp(s(0, by, bz).getID()))
						return true;
				}
				else return true;
			}
		}
		else
		{
			if (LightidityComp(sector(bx+1, by, bz).getID()))
				return true;
		}
		
		if (by == 0)
		{
			if (sector.getY() > 0)
			{
				Sector& s = Sectors(sector.getX(), sector.getY()-1, sector.getZ());
				if (s.contents > Sector::CONT_UNKNOWN)
				{
					if (LightidityComp(s(bx, Sector::BLOCKS_Y-1, bz).getID()))
						return true;
				}
				else return true;
			}
		}
		else
		{
			if (LightidityComp(sector(bx, by-1, bz).getID()))
				return true;
		}
		
		if (by == Sector::BLOCKS_Y-1)
		{
			if (sector.getY() < Sectors.getY()-1)
			{
				Sector& s = Sectors(sector.getX(), sector.getY()+1, sector.getZ());
				if (s.contents > Sector::CONT_UNKNOWN)
				{
					if (LightidityCompTop(s(bx, 0, bz).getID()))
						return true;
				}
				else return true;
			}
		}
		else
		{
			if (LightidityCompTop(sector(bx, by+1, bz).getID()))
				return true;
		}
		
		if (bz == 0)
		{
			if (sector.getZ() > 0)
			{
				Sector& s = Sectors(sector.getX(), sector.getY(), sector.getZ()-1);
				if (s.contents > Sector::CONT_UNKNOWN)
				{
					if (LightidityComp(s(bx, by, Sector::BLOCKS_XZ-1).getID()))
						return true;
				}
				else return true;
			}
		}
		else
		{
			if (LightidityComp(sector(bx, by, bz-1).getID()))
				return true;
		}
		
		if (bz == Sector::BLOCKS_XZ-1)
		{
			if (sector.getZ() < Sectors.getXZ()-1)
			{
				Sector& s = Sectors(sector.getX(), sector.getY(), sector.getZ()+1);
				if (s.contents > Sector::CONT_UNKNOWN)
				{
					if (LightidityComp(s(bx, by, 0).getID()))
						return true;
				}
				else return true;
			}
		}
		else
		{
			if (LightidityComp(sector(bx, by, bz+1).getID()))
				return true;
		}
		return false;
	}
	
	void Torchlight::lightGatherer(Sector& sector, LightList& list)
	{
		// create boundries
		int x0 = (sector.getX() - 1 < 0) ? 0 : sector.getX() - 1;
		int x1 = (sector.getX() + 1 >= Sectors.getXZ()) ? Sectors.getXZ()-1 : sector.getX() + 1;
		int y0 = (sector.getY() - 2 < 0) ? 0 : sector.getY() - 2;
		int y1 = (sector.getY() + 2 >= Sectors.getY() ) ? Sectors.getY()-1  : sector.getY() + 2;
		int z0 = (sector.getZ() - 1 < 0) ? 0 : sector.getZ() - 1;
		int z1 = (sector.getZ() + 1 >= Sectors.getXZ()) ? Sectors.getXZ()-1 : sector.getZ() + 1;
		
		// clear light list
		list.lights.clear();
		
		for (int x = x0; x <= x1; x++)
		for (int z = z0; z <= z1; z++)
		for (int y = y0; y <= y1; y++)
		{
			Sector& s2 = Sectors(x, y, z);
			
			if (s2.hasLight != 1 && s2.contents == Sector::CONT_SAVEDATA)
			if (s2.lightCount())
			{
				// backwards counter used for early exiting
				short total = s2.lightCount();
				bool maxsolid = (s2.solidFlags() == Sector::MAX_HARDSOLID);
				
				// iterate all blocks in sector
				for (int bx = 0; bx < Sector::BLOCKS_XZ; bx++)
				for (int bz = 0; bz < Sector::BLOCKS_XZ; bz++)
				for (int by = 0; by < Sector::BLOCKS_Y;  by++)
				{
					// if sector is totally encompassed by solid blocks
					if (maxsolid)
					{
						// iterate only edges
						if (bx != 0 && bx != Sector::BLOCKS_XZ-1 &&
							by != 0 && by != Sector::BLOCKS_Y-1 &&
							bz != 0 && bz != Sector::BLOCKS_XZ-1) continue;
					}
					
					Block& block = s2(bx, by, bz);
					if (isLight(block.getID()))
					{
						short lem = getEmitterId(block.getID());
						const lightdata_t& id = getEmitter(lem);
						
						bool test = true;
						if (&sector != &s2) test = (sector.distanceTo(s2, bx, by, bz) < id.reach);
						
						if (test)
						{
							if (lightGathererHelper(s2, bx, by, bz))
							{
								// add light
								list.lights.emplace_back(
									&s2, (short)bx, (short)by, (short)bz, lem
								);
								
							} // visibility culling
							
						} // emission reach test
						
						// decrease known total, keeping track of lights
						total -= 1;
						if (total == 0) goto gatherNextSector; // exit when no lights left
					}
					
				} // iterate blocks
				
			} // sector has emissive blocks
			
			gatherNextSector:;
			
		} // iterate sectors
		
		// set gathered flag
		list.gathered = true;
		
		// avoid counting again, unless absolutely necessary
		if (list.lights.size())
		{
			sector.hasLight = 2; // has lights
		}
		else
		{
			sector.hasLight = 1; // has no lights
		}
	}
	
}
