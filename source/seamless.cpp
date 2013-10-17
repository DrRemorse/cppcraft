/**
 * Seamless transition function
 * 
 * Moves the player back exactly one sector, while adding a new wall of sectors
 * ahead of him, and removing the wall behind him
 * 
 * returns the number of transitions that were completed
**/

#include "seamless.hpp"

#include "columns.hpp"
#include "flatlands.hpp"
#include "camera.hpp"
#include "minimap.hpp"
#include "player.hpp"
#include "player_logic.hpp"
#include "precompq.hpp"
#include "sector.hpp"
#include "sun.hpp"
#include "threading.hpp"
#include "world.hpp"
#include "worldbuilder.hpp"

namespace cppcraft
{
	// length player has to travel for a transition to take place
	const int Seamless::OFFSET = Sector::BLOCKS_XZ;
	
	// runs seamless() until no more transitions happen
	// essentially moving the player until he is on a local grid, near center
	bool Seamless::run()
	{
		// only run seamless if the player actually moved, one way or another
		// -=- COULD DO STRANGE STUFF -=-
		bool running = true;
		bool seam = false;
		while(running)
		{
			// transition world
			running = Seamless::seamlessness();
			// decrease worldbuilder radius
			if (running)
			{
				worldbuilder.seamResponse();
				seam = true;
			}
		}
		return seam;
	}
	
	class Seamstress
	{
	public:
		static void resetSector(Sector& oldpointer);
		static void updateSector(int x, int y, int z);
	};
	
	inline void Seamstress::resetSector(Sector& oldpointer)
	{
		// NOTE: GRIDTESTING DEALLOCATES VBO DATA
		//       IN ANOTHER THREAD! DONT REMOVE VBODATA!
		
		// invalidate sector, which makes it regenerate
		oldpointer.invalidate();
	}
	
	void Seamstress::updateSector(int x, int y, int z)
	{
		// sectors are on the heap, nonetheless returning as reference
		Sector& s = Sectors(x, y, z);
		// if this sector is beyond needing generation
		if (s.contents == Sector::CONT_SAVEDATA)
		{
			// newly introduced sectors can have additional torchlight
			// this sector has NEW exposure to lights it didn't have before
			s.hasLight = 0;
			// recompile sector mesh
			s.progress = Sector::PROG_NEEDRECOMP;
		}
	}
	
	// things that must be done prior to moving the world
	void Seamless::seamless_preconditions()
	{
		// finish all running precompiler threads
		//precompq.finish();
	}
	
	// big huge monster function
	// writeme
	bool Seamless::seamlessness()
	{
		int x, y, z;
		int halfworld = Sectors.getXZ() * Sector::BLOCKS_XZ / 2;
		bool returnvalue = false;
		
		// if player is beyond negative seam offset point on x axis
		if (player.X <= halfworld - Seamless::OFFSET)
		{
			seamless_preconditions();
			
			mtx.sectorseam.lock();
			
			// move player forward one sector (in blocks)
			player.X += Sector::BLOCKS_XZ;
			player.snapX = player.X;
			player.JustMoved = true;
			// offset world x by -1
			world.worldX -= 1;
			
			// only 25% left on the negative side
			for (z = 0; z < Sectors.getXZ(); z++)
			{
				for (y = 0; y < Sectors.getY(); y++)
				{
					// remember old sector, at the end of x-axis
					Sector* oldpointer = Sectors.getSectorPtr(Sectors.getXZ()-1, y, z);
					
					#ifdef USE_WORK
						// remove all jobs currently running on this sector
						if (oldpointer->haswork) workq.removeWork(oldpointer);
					#endif
					
					// move forward on the x-axis
					for (x = Sectors.getXZ() - 1; x >= 1; x--)
					{
						// set sector (x, y, z) to sector (x-1, y, z)
						Sectors.set(x,y,z, x-1,y,z);
						// update x-position for each sector
						Sectors(x, y, z).x = x;
					}
					// set first sector on x-axis to old pointer
					Sectors.set(0, y, z, oldpointer);
					// update x-position
					oldpointer->x = 0;
					// reset it completely
					Seamstress::resetSector(*oldpointer);
					// flag neighboring sector as dirty, if necessary
					Seamstress::updateSector(1, y, z);
					
					// fuck if i know
					if (thesun.getAngle().x < 0.0)
					{
						Seamstress::updateSector(2, y, z);
						Seamstress::updateSector(3, y, z);
					}
					
				} // sectors y
				
				// move columns backwards -x
				for (y = 0; y < ColumnsContainer::COLUMNS_Y; y++)
				{
					Column* tc = Columns.manipulate(Sectors.getXZ() - 1, y, z);
					
					for (x = Sectors.getXZ()-1; x >= 1; x--)
					{
						Columns.manipulate(x, y, z) = Columns.manipulate(x - 1, y, z);
					}
					
					Columns.manipulate(0, y, z) = tc;
					tc->reset();
				}
				
				// move flatlands backwards -x
				FlatlandSector* flatl = Flatlands.manipulate(Sectors.getXZ() - 1, z);
				
				for (x = Sectors.getXZ()-1; x >= 1; x--)
				{
					Flatlands.manipulate(x, z) = Flatlands.manipulate(x - 1, z);
				}
				// re-set first on x-axis
				Flatlands.manipulate(0, z) = flatl;
				
			} // sectors z
			
			// minimap rollover +x
			minimap.roll(-1, 0);
			
			// flag frustum as needing recalculation
			camera.recalc = true;
			
			mtx.sectorseam.unlock();
			returnvalue = true;
		}
		else if (player.X >= halfworld + Seamless::OFFSET)
		{
			seamless_preconditions();
			
			mtx.sectorseam.lock();
			
			// move player back one sector (in blocks)
			player.X -= Sector::BLOCKS_XZ;
			player.snapX = player.X;
			player.JustMoved = true;
			// offset world x by +1
			world.worldX += 1;
			
			// only 25% left on the positive side
			for (z = 0; z < Sectors.getXZ(); z++)
			{
				for (y = 0; y < Sectors.getY(); y++)
				{
					// remember first sector on x-axis
					Sector* oldpointer = Sectors.getSectorPtr(0, y, z);
					
					#ifdef USE_WORK
						// remove all jobs currently running on this sector
						if (oldpointer->haswork) Work::removeWork(oldpointer);
					#endif
					
					for (x = 0; x < Sectors.getXZ()-1; x++)
					{
						Sectors.set(x,y,z, x+1,y,z);
						// update x-position for each sector
						Sectors(x, y, z).x = x;
					}
					
					// move oldpointer-sector to end of x-axis
					Sectors.set(Sectors.getXZ()-1, y, z,  oldpointer);
					// update x-position
					oldpointer->x = Sectors.getXZ() - 1;
					
					// reset sector completely
					Seamstress::resetSector(*oldpointer);
					// update neighbor
					Seamstress::updateSector(Sectors.getXZ() - 2, y, z);
					
					if (thesun.getAngle().x > 0.0)
					{
						Seamstress::updateSector(Sectors.getXZ() - 3, y, z);
						Seamstress::updateSector(Sectors.getXZ() - 4, y, z);
					}
					
				} // sectors y
				
				// move columns forwards +x
				for (y = 0; y < ColumnsContainer::COLUMNS_Y; y++)
				{
					Column* tc = Columns.manipulate(0, y, z);
					
					for (x = 0; x < Sectors.getXZ()-1; x++)
					{
						Columns.manipulate(x, y, z) = Columns.manipulate(x + 1, y, z);
					}
					
					Columns.manipulate(Sectors.getXZ()-1, y, z) = tc;
					tc->reset();
				} // columns
				
				// move flatlands forwards +x
				FlatlandSector* flatl = Flatlands.manipulate(0, z);
				for (x = 0; x < Sectors.getXZ()-1; x++)
				{
					Flatlands.manipulate(x, z) = Flatlands.manipulate(x + 1, z);
				}
				Flatlands.manipulate(Sectors.getXZ()-1, z) = flatl;
				
			} // sectors z
			
			// minimap rollover -x
			minimap.roll(1, 0);
			
			// flag camera for needing recalculation
			camera.recalc = true;
			
			mtx.sectorseam.unlock();
			returnvalue = true;
			
		} // seamless +/- x
		
		// -== Z axis ==-
		
		if (player.Z <= halfworld - Seamless::OFFSET)
		{
			seamless_preconditions();
			
			mtx.sectorseam.lock();
			
			// offset player +z
			player.Z += Sector::BLOCKS_XZ;
			player.snapZ = player.Z;
			player.JustMoved = true;
			// offset world -z
			world.worldZ -= 1;
			
			// only 25% left on the negative side
			for (x = 0; x < Sectors.getXZ(); x++)
			{
				for (y = 0; y < Sectors.getY(); y++)
				{
					// recursively move the sector
					Sector* oldpointer = Sectors.getSectorPtr(x, y, Sectors.getXZ() - 1);
					
					#ifdef USE_WORK
						if (oldpointer->haswork) RemoveWork(oldpointer);
					#endif
					
					for (z = Sectors.getXZ() - 1; z >= 1; z--)
					{
						Sectors.set(x, y, z, x, y, z - 1);
						Sectors(x, y, z).z = z;
					}
					// generate new sector
					Sectors.set(x, y, 0, oldpointer);
					oldpointer->z = 0;
					
					// reset oldpointer sector
					Seamstress::resetSector(*oldpointer);
					
					// only need to update 1 row for Z
					Seamstress::updateSector(x, y, 1);
					
				} // sectors y
				
				// move columns backwards -z
				for (y = 0; y < Columns.COLUMNS_Y; y++)
				{
					Column* tc = Columns.manipulate(x, y, Sectors.getXZ()-1);
					
					for (z = Sectors.getXZ()-1; z >= 1; z--)
					{
						Columns.manipulate(x, y, z) = Columns.manipulate(x, y, z-1);
					}
					
					Columns.manipulate(x, y, 0) = tc;
					tc->reset();
					
				} // columns
				
				// move flatlands backwards -z
				FlatlandSector* flatl = Flatlands.manipulate(x, Sectors.getXZ() - 1);
				
				for (z = Sectors.getXZ()-1; z >= 1; z--)
				{
					Flatlands.manipulate(x, z) = Flatlands.manipulate(x, z - 1);
				}
				Flatlands.manipulate(x, 0) = flatl;
				
			} // sectors x
			
			// minimap rollover +z
			minimap.roll(0, -1);
			
			// flag camera for needing recalculation
			camera.recalc = true;
			
			mtx.sectorseam.unlock();
			return true;
		}
		else if (player.Z >= halfworld + Seamless::OFFSET)
		{
			seamless_preconditions();
			
			mtx.sectorseam.lock();
			
			// move player backward on the Z axis
			player.Z -= Sector::BLOCKS_XZ;
			player.snapZ = player.Z;
			player.JustMoved = true;
			// move world forward on the Z axis
			world.worldZ += 1;
			
			// move sectors forwards +z (and rollback last line)
			for (x = 0; x < Sectors.getXZ(); x++)
			{
				for (y = 0; y < Sectors.getY(); y++)
				{
					Sector* oldpointer = Sectors.getSectorPtr(x, y, 0);
					
					#ifdef USE_WORK
						if (oldpointer->haswork) RemoveWork(oldpointer);
					#endif
					
					// recursively move sectors
					for (z = 0; z < Sectors.getXZ()-1; z++)
					{
						Sectors.set(x,y,z, x,y,z+1);
						Sectors(x, y, z).z = z;
					}
					// generate new sector
					Sectors.set(x, y, Sectors.getXZ()-1, oldpointer);
					oldpointer->z = Sectors.getXZ()-1;
					
					// reset Sector* oldpointer
					Seamstress::resetSector(*oldpointer);
					
					// only need to update 1 row for Z
					Seamstress::updateSector(x, y, Sectors.getXZ() - 2);
					
				} // sectors y
				
				// move columns forwards +z
				for (y = 0; y < Columns.COLUMNS_Y; y++)
				{
					Column* tc = Columns.manipulate(x, y, 0);
					
					for (z = 0; z < Sectors.getXZ()-1; z++)
					{
						Columns.manipulate(x, y, z) = Columns.manipulate(x, y, z + 1);
					}
					
					Columns.manipulate(x, y, Sectors.getXZ() - 1) = tc;
					tc->reset();
					
				} // columns
				
				// move flatlands forwards +z
				FlatlandSector* flatl = Flatlands.manipulate(x, 0);
				
				for (z = 0; z < Sectors.getXZ()-1; z++)
				{
					Flatlands.manipulate(x, z) = Flatlands.manipulate(x, z + 1);
				}
				Flatlands.manipulate(x, Sectors.getXZ()-1) = flatl;
				
			} // sectors x
			
			// minimap rollover -z
			minimap.roll(0, 1);
			
			// flag camera for needing recalculation
			camera.recalc = true;
			
			mtx.sectorseam.unlock();
			return true;
			
		} // seamless +/- z
		
		// final returnvalue
		return returnvalue;
		
	} // seamlessness
	
}
