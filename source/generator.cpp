#include "generator.hpp"

#include "library/config.hpp"
#include "library/log.hpp"
#include "chunks.hpp"
#include "minimap.hpp"
#include "player.hpp"
#include "sectors.hpp"
#include "torchlight.hpp"
#include "world.hpp"

// load compressor last
#include "compressor.hpp"

using namespace library;

namespace cppcraft
{
	unsigned int g_fres[Chunks::chunk_size][Sectors.SECTORS_Y][Chunks::chunk_size];
	unsigned int g_compres[Chunks::chunk_size][Chunks::chunk_size];
	
	void Generator::loadSector(Sector& sector, std::ifstream& file, unsigned int PL)
	{
		// start by setting sector as having savedata
		sector.contents = Sector::CONT_SAVEDATA;
		
		// load sector, which can return a nullsector
		chunks.loadSector(sector, file, PL);
		
		// if the sector still has saved data, process sector and propagate light
		if (sector.contents == Sector::CONT_SAVEDATA)
		{
			// (eventually) place sector in precompiler queue
			if (sector.progress != Sector::PROG_NEEDRECOMP)
			{
				sector.progress = Sector::PROG_NEEDRECOMP;
			}
			// update sector neighbors, if there are emissive lights on this sector
			if (sector.torchlight)
			{
				// worst-case emissive block reach (_TORCH)
				torchlight.lightSectorUpdates(sector, _TORCH, false);
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
	 * Generator will round sector(x, z) down to the nearest chunk,
	 * and attempt to load as many sectors from this chunk as possible in one go.
	 * Will first attempt to decompress a compressed column, then replace with modified sectors as they appear
	**/
	void Generator::generate(Sector& sector)
	{
		
		if (sector.progress != Sector::PROG_NEEDGEN)
		{
			logger << Log::WARN << "Generator::generate(): sector did not need gen" << Log::ENDL;
			return;
		}
		
		// open this chunks .compressed file
		const std::string& worldFolder = world.worldFolder();
		std::string sectorString = chunks.getSectorString(sector);
		
		std::ifstream cf;
		cf.open(worldFolder + "/" + sectorString + ".compressed", std::ios::in | std::ios::binary);
		
		if (cf.is_open())
		{
			cf.seekg(sizeof(int));
			cf.read( (char*) g_compres, sizeof(g_compres) );
		}
		
		// open this chunks (raw) .chunk file
		std::ifstream ff;
		ff.open(worldFolder + "/" + sectorString + ".chunk", std::ios::in | std::ios::binary);
		
		if (ff.is_open())
		{
			ff.seekg(sizeof(int));
			ff.read( (char*) g_fres, sizeof(g_fres) );
		}
		
		
		// NOTE NOTE NOTE NOTE NOTE //
		//   NO EARLY EXITS HERE    //
		// NOTE NOTE NOTE NOTE NOTE //
		
		int dx = (sector.x + world.getWX()) & (Chunks::chunk_size-1);
		int dz = (sector.z + world.getWZ()) & (Chunks::chunk_size-1);
		
		int x1 = sector.x - dx;
		int x2 = x1 + Chunks::chunk_size;
		
		if (x1 < 0) x1 = 0;            // CLAMP AFTER x2 IS SET!!!
		if (x2 > Sectors.getXZ()) x2 = Sectors.getXZ();
		
		int z1 = sector.z - dz;
		int z2 = z1 + Chunks::chunk_size;
		
		if (z1 < 0) z1 = 0;            // CLAMP AFTER z2 IS SET!!!
		if (z2 > Sectors.getXZ()) z2 = Sectors.getXZ();
		
		for (int x = x1; x < x2; x++)
		{
			for (int z = z1; z < z2; z++)
			{
				Sector& firstsector = Sectors(x, 0, z);
				
				//-------------------------------------------------//
				// load only sectors that are flagged as 'unknown' //
				//-------------------------------------------------//
				
				if (firstsector.contents == Sector::CONT_UNKNOWN)
				{
					// find sectors internal chunk position
					dx = (x + world.getWX()) & (Chunks::chunk_size-1);
					dz = (z + world.getWZ()) & (Chunks::chunk_size-1);
					
					// if compressed file is open, and there was an entry
					if (cf.is_open() && (g_compres[dx][dz] != 0))
					{
						// read entire compressed column
						// compressed column also contains the flatland(x, z) for this area
						Compressor::load(cf, g_compres[dx][dz], x, z);
						
						// update minimap (colors)
						minimap.addSector(firstsector);
					}
					else
					{
						for (int y = 0; y < Sectors.getY(); y++)
						{
							// for each sector on y-axis,
							Sector& currentsector = Sectors(x, y, z);
							// clean out sector
							currentsector.clear();
						}
					}
					
					if (ff.is_open())
					{
						// load single-file
						
						for (int y = 0; y < Sectors.getY(); y++)
						{
							if (g_fres[dz][y][dx])
							{
								// for each sector on y-axis with entry
								Sector& currentsector = Sectors(x, y, z);
								// load sector using loadSectorEx method
								loadSector(currentsector, ff, g_fres[dz][y][dx]);
							}	
							
						}
						
					} // ff is open
					
				} // sector in unknown-state
				
			} // z
		} // x
		
		if (ff.is_open()) ff.close();
		if (cf.is_open()) cf.close();
		
	} // generate()
	
}
