#include "sector.hpp"

#include <library/log.hpp>
#include "generator.hpp"
#include "vertex_block.hpp"
#include <cstring>
#include <cmath>

using namespace library;

namespace cppcraft
{
	Sector::Sector(int x, int y, int z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		// initialize blocks to null
		this->blockpt = nullptr;
		//this->special = nullptr;
		
		this->render = false;          // not renderable
		this->progress = PROG_NEEDGEN; // needs to be generated
		this->contents = CONT_UNKNOWN; // unknown content
		
		this->culled  = false; // not culled / covered by other sectors
		this->hasWork = false; // no work
		this->hasLight = 0;  // unknown: we don't know if its exposed to any lights
	}
	Sector::~Sector()
	{
		delete this->blockpt;
	}
	
	void Sector::createBlocks()
	{
		if (this->blockpt)
		{
			logger << Log::ERR << "Sector::createBlocks(): Block ptr already existed: " << blockpt << Log::ENDL;
			return;
		}
		
		this->blockpt = new sectorblock_t;
	}
	
	void Sector::smartAssignBlocks(bool needBlocks)
	{
		// note: this function has no side effects, it's only supposed
		// to assign blocks, and (possibly) clear them for immediate usage
		if (progress == PROG_NEEDGEN)
		{
			// generate blocks now, no flag CHANGES
			Generator::generate(*this, nullptr, 0);
		}
		
		// if the sector doesn't really need blocks, just exit
		if (needBlocks == false) return;
		
		// create blocks, if wanted
		if (hasBlocks() == false)
		{
			createBlocks();
			// then clear the blocks completely
			memset(blockpt, 0, sizeof(sectorblock_t));
			// set to savedata
			this->contents = CONT_SAVEDATA;
		}
	}
	
	int Sector::countLights()
	{
		if (hasBlocks() == false) throw std::string("Sector::countLights(): Sector had no blocks");
		
		Block* block = &blockpt->b[0][0][0];
		Block* lastBlock = block + BLOCKS_XZ * BLOCKS_XZ * BLOCKS_Y;
		int lights = 0;
		
		for (; block < lastBlock; block++)
		{
			if (isLight(block->getID())) lights++;
		}
		
		blockpt->lights = lights;
		return lights;
	}
	
	float Sector::distanceTo(const Sector& sector, int bx, int by, int bz) const
	{
		// centroidal
		int dx = ((getX() - sector.getX()) << BLOCKS_XZ_SH) + (BLOCKS_XZ / 2 - bx);
		int dy = ((getY() - sector.getY()) << BLOCKS_Y_SH)  + (BLOCKS_Y  / 2 - by);
		int dz = ((getZ() - sector.getZ()) << BLOCKS_XZ_SH) + (BLOCKS_XZ / 2 - bz);
		
		return sqrtf(dx*dx + dy*dy + dz*dz) - (BLOCKS_XZ / 2) * sqrtf(3.0);
	}
	
	void Sector::clear()
	{
		render = false;
		// and nullsectors are always "compiled"
		progress = PROG_COMPILED;
		// clearing its block contents
		contents = CONT_NULLSECTOR;
		
		// clear many flags, ... bite me
		culled = false;
		hasWork = false;
		hasLight = 1;     // no lights (NOTE: MAY BE WRONG)
		
		// remove additional data
		/*if (special)
		//{
		//	free(special);
		//	special = nullptr;
		}*/
		
		delete blockpt;
		blockpt = nullptr;
	}
	
	void Sector::invalidate()
	{
		progress = PROG_NEEDGEN;
		contents = CONT_UNKNOWN;
		render   = false;
		culled   = false;
	}
	
}
