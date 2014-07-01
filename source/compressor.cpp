#include "compressor.hpp"

#include <library/log.hpp>
#include <library/compression/lzo.hpp>
#include "compressor_rle.hpp"
#include "sectors.hpp"
#include "flatlands.hpp"

#include <cstring>

using namespace library;

namespace cppcraft
{
	library::LZO compressor;
	lzo_bytep compressor_databuffer;
	
	struct compressed_datalength_t
	{
		unsigned short lzoSize;
		unsigned short sectors;
	};
	
	void Compressor::init()
	{
		logger << Log::INFO << "* Initializing compressor" << Log::ENDL;
		
		const int compressed_column_size = FlatlandSector::FLATLAND_SIZE + Sectors.getY() * sizeof(Sector::sectorblock_t);
		
		// initialize LZO
		compressor.init(compressed_column_size);
		
		// allocate towering buffer
		compressor_databuffer = (lzo_bytep) malloc(compressed_column_size * sizeof(lzo_byte));
		if (compressor_databuffer == nullptr)
			throw std::string("Chunks:compressorInit(): Compressor databuffer was null");
	}
	
	void Compressor::load(std::ifstream& File, int PL, int x, int z)
	{
		// read datalength
		compressed_datalength_t datalength;
		
		File.seekg(PL);
		File.read( (char*) &datalength, sizeof(compressed_datalength_t) );
		
		if (datalength.lzoSize == 0)
		{
			// reset flatlands
			memset( flatlands(x, z).fdata, 0,  FlatlandSector::FLATLAND_SIZE);
			// not generated world
			for (int y = 0; y < Sectors.getY(); y++)
			{
				// clear all sectors in this (x, z)
				Sectors(x, y, z).clear();
			}
			// exit early
			return;
		}
		
		// go past first struct
		File.seekg(PL+sizeof(compressed_datalength_t));
		// read entire compressed block
		File.read((char*) compressor_databuffer, datalength.lzoSize);
		
		// decompress data
		if (compressor.decompress2a(compressor_databuffer, datalength.lzoSize) == false)
		{
			logger << Log::ERR << "Compressor::decompress(): Failed to decompress data" << Log::ENDL;
			throw std::string("Compressor::decompress(): Failed to decompress data");
		}
		
		lzo_bytep cpos = compressor.getData();
		
		// copy over flatland struct
		memcpy (flatlands(x, z).fdata, cpos, FlatlandSector::FLATLAND_SIZE);
		
		// move to first sectorblock
		cpos += sizeof(FlatlandSector::fdata);
		
		Sector* sbase = &Sectors(x, 0, z);
		
		for (int y = 0; y < Sectors.getY(); y++)
		{
			if (y < datalength.sectors)
			{
				// check if any blocks are present
				if (rle.hasBlocks(cpos))
				{
					// copy data to engine side
					if (sbase[y].hasBlocks() == false)
						sbase[y].createBlocks();
					
					// decompress directly onto sectors sectorblock
					rle.decompress(cpos, *sbase[y].blockpt);
					
					// set sector flags (based on sectorblock flags)
					// flag sector for mesh assembly (next stage in pipeline)
					sbase[y].progress = Sector::PROG_NEEDRECOMP;
					// set sector-has-data flag
					sbase[y].contents = Sector::CONT_SAVEDATA;
					// flag as needing light gathering
					sbase[y].hasLight = 0;
				}
				else
				{	// had no blocks, just null it
					sbase[y].clear();
				}
				
				// go to next RLE compressed sector
				cpos += rle.getSize();
			}
			else
			{	// out of bounds, just null it
				sbase[y].clear();
			}
			
		} // y
		
	} // loadCompressedColumn
	
}
