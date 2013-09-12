#include "compressor_rle.hpp"

#include "library/log.hpp"

using namespace library;

namespace cppcraft
{
	RLECompressor rle;
	
	bool RLECompressor::hasBlocks(unsigned char* data)
	{
		// set RLEHeader internally, so that further info can be retrieved later on
		// CAREFUL when removing the next line, there is a dependency in the compressor
		rh = (RLEHeader*) data;
		
		return rh->getBlocks() != 0;
	}
	
	void RLECompressor::decompress(unsigned char* data, Sector::sectorblock_t& sb)
	{
		// first treat data as a RLEHeader
		rh = (RLEHeader*) data;
		data += sizeof(RLEHeader);
		
		// get entry data
		RLEEntry* entries = (RLEEntry*) data;
		data += rh->getEntries() * sizeof(RLEEntry);
		
		// get palette data
		Block* palette = (Block*) data;
		
		// set sectorblock info
		sb.blocks = rh->getBlocks();
		sb.lights = rh->getLights();
		sb.hardsolid = rh->getHardsolid();
		
		Block* block = sb.b[0][0];
		
		// fill sectorblock with palette from entries
		for (int entry = 0; entry < rh->getEntries(); entry++)
		{
			while(entries[entry].count--)
			{
				*block = palette[ entries[entry].palid ];
				block++;
			}
		}
	}
	
}