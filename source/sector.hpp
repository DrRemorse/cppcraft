#ifndef SECTOR_HPP
#define SECTOR_HPP

#include "blocks.hpp"
#include "renderconst.hpp"

namespace cppcraft
{
	struct vertex_t;
	
	struct vbodata_t
	{
		vertex_t* pcdata;
		unsigned short bufferoffset[RenderConst::MAX_UNIQUE_SHADERS];
		unsigned short vertices	[RenderConst::MAX_UNIQUE_SHADERS];
	};
	
	class Sector
	{
	public:
		// sector size constants
		static const int BLOCKS_XZ = 16;
		static const int BLOCKS_Y  =  8;
		// sector bitshift size constants
		static const int BLOCKS_XZ_SH = 4;
		static const int BLOCKS_Y_SH  = 3;
		// bitmask for a fully hidden sector
		static const int MAX_HARDSOLID = 63;
		
		// sector-block, a 3D volume of blocks
		#pragma pack(push, 2)
		typedef struct sectorblock_t
		{
			Block b[BLOCKS_XZ][BLOCKS_XZ][BLOCKS_Y] __attribute__((aligned(2)));
			short blocks;
			short lights;
			unsigned char hardsolid;
			unsigned char special;
			unsigned short version;
			
		} sectorblock_t;
		#pragma pack(pop)
		
		// data/blocks
		sectorblock_t* blockpt;
		// opengl vbo data
		vbodata_t* vbodata;
		// special datablock
		//specialptr special;
		
		// grid position
		short x, y, z;
		
		// flags
		bool render;   // renderable
		char progress; // progress_t
		char contents; // sectorcontents_t
		bool culled;   // surrounded by solids
		bool hasWork;
		// 0 = unknown (not computed), 1 = found no lights, 2 = found light(s)
		char hasLight;
		
		enum progress_t
		{
			PROG_NEEDGEN,
			PROG_NEEDRECOMP,
			PROG_RECOMPILE,
			PROG_RECOMPILING,
			PROG_NEEDAO,
			PROG_AO,
			PROG_NEEDCOMPILE,
			PROG_COMPILED
		};
		
		enum sectorcontents_t
		{
			CONT_NULLSECTOR,
			CONT_UNKNOWN,
			CONT_SAVEDATA
		};
		
		// creates a sector with location (x, y, z)
		Sector(int x, int y, int z);
		
		// returns true if the sector has been assigned a sectorblock
		inline bool hasBlocks() const
		{
			return (this->blockpt != nullptr);
		}
		
		// returns true if there is a point in trying to recompile this sector
		inline bool isUpdateable() const
		{
			return (contents == CONT_SAVEDATA && culled == false);
		}
		
		void createBlocks();
		void smartAssignBlocks(bool needBlocks);
		void clear();      // clears everything! and then some!
		void invalidate(); // sector needs to be generated
		
		// returns reference to a Block at (x, y, z)
		inline Block& operator() (int x, int y, int z)
		{
			return this->blockpt->b[x][z][y];
		}
		
		// sector boolean equality tests
		inline bool operator == (const Sector& sector) const
		{
			return (this == &sector);
		}
		inline bool operator != (const Sector& sector) const
		{
			return (this != &sector);
		}
		
		// distance to another sector (in block units)
		float distanceTo(const Sector& sector, int bx, int by, int bz) const;
		
		// torchlight related
		int countLights(); // recounts lights internally AND returns that count
	};
	
}

#endif
