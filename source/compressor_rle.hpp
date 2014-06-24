#ifndef COMPRESSOR_RLE_HPP
#define COMPRESSOR_RLE_HPP

#include "sector.hpp"

namespace cppcraft
{
	
	struct RLEEntry
	{
		unsigned short palid; // palette id (corresponding to a block)
		unsigned short count; // number of recurring blocks
	};
	
	class RLEHeader
	{
	private:
		unsigned int commonData;
		//uinteger blocks    : 12
		//uinteger lights    : 12
		//uinteger hardsolid :  8
		unsigned short palettes; // number of palettes
		unsigned short entries;  // number of palette entries
		
	public:
		inline unsigned int getBlocks() const
		{
			return commonData & 4095;
		}
		inline unsigned int getLights() const
		{
			return (commonData >> 12) & 4095;
		}
		inline unsigned int getHardsolid() const
		{
			return commonData >> 24;
		}
		
		inline unsigned short getPalettes() const
		{
			return palettes;
		}
		inline unsigned short getEntries() const
		{
			return entries;
		}
		
	};
	
	class RLECompressor
	{
	public:
		void zero();
		void compress(Sector::sectorblock_t& sb);
		
		bool hasBlocks(unsigned char* data);
		void decompress(unsigned char* data, Sector::sectorblock_t& sb);
		
		// total compressed size
		inline int getSize() const
		{
			return sizeof(RLEHeader) + getDataSize();
		}
		// compressed data size
		inline int getDataSize() const
		{
			return rh->getPalettes() * sizeof(block_t) + rh->getEntries() * sizeof(RLEEntry);
		}
		
	private:
		RLEHeader* rh;
		
	};
	extern RLECompressor rle;
}

#endif
