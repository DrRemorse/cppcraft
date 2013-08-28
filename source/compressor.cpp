#include "compressor.hpp"

#include "library/log.hpp"
#include "library/compression/lzo.hpp"
#include "sectors.hpp"
#include "flatlands.hpp"

#include <string>

using namespace library;

namespace cppcraft
{
	const int compressed_column_size = sizeof(FlatlandSector) + Sectors.getY() * sizeof(Sector::sectorblock_t);
	
	library::LZO compressor;
	lzo_bytep compressor_databuffer;
	
	typedef struct
	{
		unsigned short lzoSize;
		unsigned short sectors;
		
	} compressed_datalength_t;
	
	
	void Compressor::init()
	{
		// do nothing
		logger << Log::INFO << "* Initializing compressor" << Log::ENDL;
		
		// initialize LZO
		compressor.init(compressed_column_size);
		
		// allocate towering buffer
		compressor_databuffer = (lzo_bytep) malloc(compressed_column_size * sizeof(lzo_byte));
		if (compressor_databuffer == nullptr)
			throw "Chunks:compressorInit(): Compressor databuffer was null";
	}
	
	void Compressor::load(std::ifstream& File, int PL, int x, int z)
	{
		// read datalength
		compressed_datalength_t datalength;
		
		File.seekg(PL-1);
		File.read( (char*) &datalength, sizeof(compressed_datalength_t) );
		
		if (datalength.sectors == 0 || datalength.lzoSize == 0)
		{
			// not generated world
			for (int y = 0; y < Sectors.getY(); y++)
			{
				// current sector
				Sector& sector = Sectors(x, y, z);
				sector.clear();
			}
			// exit early
			return;
		}
		
		// go past first struct
		File.seekg(PL-1+sizeof(compressed_datalength_t));
		// read entire compressed block
		File.read((char*) compressor_databuffer, datalength.lzoSize);
		
		// decompress data
		if (compressor.decompress(compressor_databuffer, datalength.lzoSize) == false)
		{
			throw std::string("Compressor::decompress() Failed to decompress data");
		}
		
		lzo_bytep cpos = compressor.getData();
		
		// copy over flatland struct
		memcpy (Flatlands(x, z).fdata, cpos, sizeof(FlatlandSector::fdata));
		
		// move to first sectorblock
		cpos += sizeof(FlatlandSector);
		
		for (int y = 0; y < Sectors.getY(); y++)
		{
			// current sector
			Sector& sector = Sectors(x, y, z);
			
			if (y < datalength.sectors)
			{
				// current sectorblock
				Sector::sectorblock_t* b = (Sector::sectorblock_t*) cpos;
				
				// check if any blocks are present
				if (b->blocks)
				{
					// copy data to engine side
					if (sector.blockpt == nullptr)
						sector.createBlocks();
					
					memcpy (sector.blockpt, cpos, sizeof(Sector::sectorblock_t));
					
					// set sector flags (based on sectorblock flags)
					sector.hardsolid  = b->hardsolid;
					sector.torchlight = b->lights;
					sector.haslight   = 0; // flag as needing light gathering
					
					// set sector-has-data flag
					sector.contents = Sector::CONT_SAVEDATA;
					// flag sector for mesh assembly pipeline
					sector.progress = Sector::PROG_NEEDRECOMP;
				}
				else
				{	// had no blocks, just null it
					sector.clear();
				}
				
				// go to next sectorblock
				cpos += sizeof(Sector::sectorblock_t);
			}
			else
			{	// out of bounds, just null it
				sector.clear();
			}
			
		} // y
		
	} // loadCompressedColumn
	
}
