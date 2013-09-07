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
		inline unsigned int getBlocks() const noexcept
		{
			return commonData & 4095;
		}
		inline unsigned int getLights() const noexcept
		{
			return (commonData >> 12) & 4095;
		}
		inline unsigned int getHardsolid() const noexcept
		{
			return commonData >> 24;
		}
		
		inline unsigned short getPalettes() const noexcept
		{
			return palettes;
		}
		inline unsigned short getEntries() const noexcept
		{
			return entries;
		}
		
	};
	
	class RLECompressor
	{
		RLEHeader* rh;
		
	public:
		void zero();
		void compress(Sector::sectorblock_t* sb);
		
		bool hasBlocks(unsigned char* data);
		void decompress(unsigned char* data, Sector::sectorblock_t& sb);
		
		// total compressed size
		int getSize() const noexcept
		{
			return sizeof(RLEHeader) + getDataSize();
		}
		// compressed data size
		int getDataSize() const noexcept
		{
			return rh->getPalettes() * sizeof(Block) + rh->getEntries() * sizeof(RLEEntry);
		}
	};
	extern RLECompressor rle;
}

#endif
