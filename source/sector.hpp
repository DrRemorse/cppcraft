#ifndef SECTOR_HPP
#define SECTOR_HPP

#include "blocks.hpp"
#include "renderconst.hpp"

namespace cppcraft
{
	struct vertex_t;
	
	typedef struct
	{
		unsigned short  bufferoffset[RenderConst::MAX_UNIQUE_SHADERS];
		unsigned short  vertices	[RenderConst::MAX_UNIQUE_SHADERS];
		vertex_t* pcdata;
	} vbodata_t;
	
	class Sector
	{
	public:
		// static sector size constants
		static const int BLOCKS_XZ = 16;
		static const int BLOCKS_Y  =  8;
		
		// sector-block, 3D volume of Block
		#pragma pack(push, 2)
		typedef struct
		{
			Block b[BLOCKS_XZ][BLOCKS_XZ][BLOCKS_Y] __attribute__((aligned(2)));
			short blocks;
			short lights;
			unsigned char hardsolid;
			unsigned char special;
			unsigned short version;
			
		} sectorblock_t;
		#pragma pack(pop)
		
		// grid position
		int x, y, z;
		
		// data/blocks
		sectorblock_t* blockpt;
		// opengl vbo data
		vbodata_t* vbodata;
		// special datablock
		//specialptr special;
		
		// flags
		unsigned char render    : 1; // renderable
		unsigned char progress  : 3; // progress_t
		unsigned char contents  : 2; // sectorcontents_t
		unsigned char precomp   : 2; // precompilation pipeline stage
		// misc flags
		unsigned char culled    : 1;
		unsigned char haswork   : 1;
		// 0 = unknown (not computed), 1 = found no lights, 2 = found light(s)
		unsigned char haslight  : 2;
		unsigned char hardsolid;
		
		short torchlight; // number of lights (if any)
		
		typedef enum
		{
			PROG_NEEDGEN,
			PROG_NEEDRECOMP,
			PROG_RECOMP,
			PROG_NEEDCOMPILE,
			PROG_COMPILED
		} progress_t;
		
		typedef enum
		{
			CONT_NULLSECTOR,
			CONT_UNKNOWN,
			CONT_SAVEDATA
		} sectorcontents_t;
		
		// returns true if the sector has been assigned a sectorblock
		Sector(int x, int y, int z);
		bool hasBlocks() const;
		void createBlocks();
		void smartAssignBlocks(bool needBlocks);
		void clear();      // clears everything! and then some!
		void invalidate(); // sector needs to be generated
		
		// returns reference to a Block at (x, y, z)
		inline Block& operator() (int x, int y, int z)
		{
			return this->blockpt->b[x][z][y];
		}
		
		// sector boolean equality test
		bool operator == (const Sector& sector) const;
		bool operator != (const Sector& sector) const;
		
		// distance to another sector (in block units)
		float distanceTo(const Sector& sector, int bx, int by, int bz) const;
		
		// torchlight related
		int countLights(); // recounts lights internally AND returns that count
		
		// number of unique block IDs a sector can have
		static const int UNIQUE_IDS_PER_SECTOR = 64;
		// bitmak for a fully hidden sector
		static const int MAX_HARDSOLID = 63;
	};
	
}

#endif
