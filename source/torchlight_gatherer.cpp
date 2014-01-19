#include "torchlight.hpp"

#include "library/log.hpp"
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
			if (sector.x > 0)
			{
				Sector& s = Sectors(sector.x-1, sector.y, sector.z);
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
			if (sector.x < Sectors.getXZ()-1)
			{
				Sector& s = Sectors(sector.x+1, sector.y, sector.z);
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
			if (sector.y > 0)
			{
				Sector& s = Sectors(sector.x, sector.y-1, sector.z);
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
			if (sector.y < Sectors.getY()-1)
			{
				Sector& s = Sectors(sector.x, sector.y+1, sector.z);
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
			if (sector.z > 0)
			{
				Sector& s = Sectors(sector.x, sector.y, sector.z-1);
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
			if (sector.z < Sectors.getXZ()-1)
			{
				Sector& s = Sectors(sector.x, sector.y, sector.z+1);
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
		#define L_G_R  LIGHT_GATHER_REACH
		const int ysize = LIGHT_GATHER_REACH;
		
		// create boundries
		int x0 = (sector.x - L_G_R < 0) ? 0 : sector.x - L_G_R;
		int x1 = (sector.x + L_G_R >= Sectors.getXZ()) ? Sectors.getXZ()-1 : sector.x + L_G_R;
		int y0 = (sector.y - ysize < 0) ? 0 : sector.y - ysize;
		int y1 = (sector.y + ysize >= Sectors.getY() ) ? Sectors.getY()-1  : sector.y + ysize;
		int z0 = (sector.z - L_G_R < 0) ? 0 : sector.z - L_G_R;
		int z1 = (sector.z + L_G_R >= Sectors.getXZ()) ? Sectors.getXZ()-1 : sector.z + L_G_R;
		
		// clear light list
		list.lights.clear();
		
		for (int x = x0; x <= x1; x++)
		for (int y = y0; y <= y1; y++)
		for (int z = z0; z <= z1; z++)
		{
			Sector& s2 = Sectors(x, y, z);
			
			if (s2.hasLight != 1 && s2.contents == Sector::CONT_SAVEDATA)
			if (s2.blockpt->lights != 0)
			{
				// backwards counter used for early exiting
				short total = s2.blockpt->lights;
				bool maxsolid = s2.blockpt->hardsolid == Sector::MAX_HARDSOLID;
				
				// iterate all blocks in sector
				for (int bx = 0; bx < Sector::BLOCKS_XZ; bx++)
				for (int by = 0; by < Sector::BLOCKS_Y;  by++)
				for (int bz = 0; bz < Sector::BLOCKS_XZ; bz++)
				{
					// if sector is totally encompassed by solid blocks
					if (maxsolid)
					{
						// iterate only edges
						if ((bx == 0 || bx == Sector::BLOCKS_XZ-1 ||
							 by == 0 || by == Sector::BLOCKS_Y-1 || 
							 bz == 0 || bz == Sector::BLOCKS_XZ-1) == false) goto nextBlock;
					}
					; /* scope */
					{
						Block& block = s2(bx, by, bz);
						if (isLight(block.getID()))
						{
							short lem = getEmitterId(block.getID());
							const lightdata_t& id = getEmitter(lem);
							
							bool test = true;
							if (sector != s2) test = (sector.distanceTo(s2, bx, by, bz) < id.reach);
							
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
							if (total == 0) goto nextSector; // exit when no lights left
						}
						
					} // scope
					nextBlock:;
					
				} // iterate blocks
				
			} // sector has emissive blocks
			
			nextSector:;
			
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
