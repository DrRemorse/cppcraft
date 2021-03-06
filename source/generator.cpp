#include "generator.hpp"

#include <library/config.hpp>
#include <library/log.hpp>
#include <library/timing/timer.hpp>
#include "chunks.hpp"
#include "flatlands.hpp"
#include "minimap.hpp"
#include "player.hpp"
#include "sectors.hpp"
#include "torchlight.hpp"
#include "world.hpp"

// load compressor last
#include "compressor.hpp"
#include <cstring>

using namespace library;

namespace cppcraft
{
	unsigned int g_fres[Chunks::CHUNK_SIZE][Sectors.SECTORS_Y][Chunks::CHUNK_SIZE];
	unsigned int g_compres[Chunks::CHUNK_SIZE][Chunks::CHUNK_SIZE];
	
	std::deque<Sector*> propagationQueue;
	
	void Generator::init()
	{
		for (int x = 0; x < Sectors.getXZ(); x++)
		for (int z = 0; z < Sectors.getXZ(); z++)
		{
			if (Sectors(x, z).contents == Sector::CONT_UNKNOWN)
			{
				Generator::generate(Sectors(x, z), nullptr, 0);
			}
		}
	}
	
	void Generator::loadSector(Sector& sector, std::ifstream& file, unsigned int PL)
	{
		// start by setting sector as having savedata
		sector.contents = Sector::CONT_SAVEDATA;
		
		// load sector, which can return a nullsector
		chunks.loadSector(sector, file, PL);
		
		// if the sector still has saved data, process sector and propagate light
		if (sector.contents == Sector::CONT_SAVEDATA)
		{
			// place sector in precompiler queue
			sector.progress = Sector::PROG_NEEDRECOMP;
			
			// update sector neighbors, if there are emissive lights on this sector
			if (sector.lightCount())
			{
				// add sector to light propagation queue
				propagationQueue.push_back(&sector);
			}
		}
		else
		{
			// clean out sector, since it didn't have savedata
			// it's probably extremely null already, but we're making sure
			sector.clear();
		}
	}
	
	/**
	 * Generator will truncate sector(x, z) down to the nearest chunk,
	 * and attempt to load as many sectors from this chunk as possible in one go.
	 * Will first attempt to decompress a compressed column, then replace with modified sectors as they appear
	**/
	bool Generator::generate(Sector& sector, Timer* timer, double timeOut)
	{
		
		if (sector.progress != Sector::PROG_NEEDGEN)
		{
			logger << Log::WARN << "Generator::generate(): sector did not need gen" << Log::ENDL;
			return false;
		}
		
		// open this chunks .compressed file
		std::string sectorString = world.worldFolder() + "/" + chunks.getSectorString(sector);
		
		std::ifstream cf(sectorString + ".compressed", std::ios::in | std::ios::binary);
		bool cf_open = false;
		
		if (cf.good())
		{
			cf.seekg(sizeof(int));
			cf.read( (char*) g_compres, sizeof(g_compres) );
			cf_open = true;
		}
		
		// open this chunks (raw) .chunk file
		std::ifstream ff(sectorString + ".chunk", std::ios::in | std::ios::binary);
		bool ff_open = false;
		
		if (ff.good())
		{
			ff.seekg(sizeof(int));
			ff.read( (char*) g_fres, sizeof(g_fres) );
			ff_open = true;
		}
		
		// NOTE NOTE NOTE NOTE NOTE //
		//   NO EARLY EXITS HERE    //
		// NOTE NOTE NOTE NOTE NOTE //
		
		int dx = (sector.getX() + world.getWX()) & (Chunks::CHUNK_SIZE-1);
		int dz = (sector.getZ() + world.getWZ()) & (Chunks::CHUNK_SIZE-1);
		
		int x1 = sector.getX() - dx;
		int x2 = x1 + Chunks::CHUNK_SIZE;
		
		if (x1 < 0) x1 = 0;            // CLAMP AFTER x2 IS SET!!!
		if (x2 > Sectors.getXZ()) x2 = Sectors.getXZ();
		
		int z1 = sector.getZ() - dz;
		int z2 = z1 + Chunks::CHUNK_SIZE;
		
		if (z1 < 0) z1 = 0;            // CLAMP AFTER z2 IS SET!!!
		if (z2 > Sectors.getXZ()) z2 = Sectors.getXZ();
		
		bool minimapUpdated = false;
		
		for (int x = x1; x < x2; x++)
		{
			for (int z = z1; z < z2; z++)
			{
				// bottom / first sector in column
				Sector& firstsector = Sectors(x, 0, z);
				
				//-------------------------------------------------//
				// load only sectors that are flagged as 'unknown' //
				//-------------------------------------------------//
				
				if (firstsector.contents == Sector::CONT_UNKNOWN)
				{
					// find sectors internal chunk position
					dx = (x + world.getWX()) & (Chunks::CHUNK_SIZE-1);
					dz = (z + world.getWZ()) & (Chunks::CHUNK_SIZE-1);
					
					// if compressed file is open, and there was an entry
					if (cf_open && (g_compres[dx][dz] != 0))
					{
						// read entire compressed column
						// compressed column also contains the flatland(x, z) for this area
						Compressor::load(cf, g_compres[dx][dz], x, z);
						
						// update minimap (colors)
						minimap.addSector(firstsector);
						minimapUpdated = true;
					}
					else
					{
						// reset flatlands
						//flatlands(x, z).reset();
						
						// for each sector on y-axis,
						for (int y = 0; y < Sectors.getY(); y++)
						{
							// clean out sector
							Sectors(x, y, z).clear();
						}
					}
					
					if (ff_open)
					{
						// load single-file
						for (int y = 0; y < Sectors.getY(); y++)
						{
							// for each sector on y-axis with entry
							if (g_fres[dz][y][dx])
							{
								// load sector using loadSectorEx method
								loadSector(Sectors(x, y, z), ff, g_fres[dz][y][dx]);
							}	
							
						}
						
					} // ff is open
					
				} // sector in unknown-state
				
				// for each finished column we can potentially exit
				// if the current time is more than the timeout value, we need to exit now
				/*if (timer)
				{
					if (timer->getDeltaTime() > timeOut)
					{
						if (minimapUpdated) minimap.setUpdated();
						return true;
					}
				}*/
			} // z
		} // x
		
		// propagate light (covers all cases, because it's AFTER loading)
		// NOTE: if there is an early exit, this can go wrong!
		while (propagationQueue.empty() == false)
		{
			Sector* lsector = propagationQueue.front();
			propagationQueue.pop_front();
			
			// worst-case emissive block reach (_TORCH)
			torchlight.lightSectorUpdates(*lsector, false);
		}
		
		if (minimapUpdated) minimap.setUpdated();
		
		if (timer)
		{
			return timer->getTime() > timeOut;
		}
		// time did not run out
		return false;
		
	} // generate()
	
}
