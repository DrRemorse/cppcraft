#include "spiders.hpp"

#include <library/log.hpp>
#include "blocks.hpp"
#include "chunks.hpp"
#include "flatlands.hpp"
#include "generator.hpp"
#include "precompq.hpp"
#include "sectors.hpp"
#include "torchlight.hpp"

using namespace library;

namespace cppcraft
{
	bool Spiders::updateBlock(int bx, int by, int bz, block_t bitfield, bool immediate)
	{
		Sector* s = spiderwrap(bx, by, bz);
		if (s == nullptr) return false;
		if (s->progress == Sector::PROG_NEEDGEN || s->contents == Sector::CONT_NULLSECTOR) return false;
		
		Block& block = s[0](bx, by, bz);
		// set bitfield directly
		block.setData(bitfield);
		
		if (immediate)
		{
			precompq.addTruckload(*s);
		}
		else s->progress = Sector::PROG_NEEDRECOMP;
		
		// write updated sector to disk
		chunks.addSector(*s);
		
		return true;
	}
	
	bool Spiders::addblock(int bx, int by, int bz, block_t id, block_t bitfield, bool immediate)
	{
		// don't use addBlock() with _AIR
		if (id == _AIR) return false;
		
		Sector* s = spiderwrap(bx, by, bz);
		if (s == nullptr) return false;
		
		if (s->progress == Sector::PROG_NEEDGEN || s->contents == Sector::CONT_NULLSECTOR) 
		{
			// we need blocks NOW
			s->smartAssignBlocks(true);
		}
		
		Block& block = s[0](bx, by, bz);
		
		// avoid re-setting same block id
		if (block.getID() == id) return false;
		
		// if the previous block was air, then we need to increase the "block count"
		if (block.getID() == _AIR)
		{
			s->blockpt->blocks += 1;
		}
		
		// set new ID, facing & special
		block.setID(id);
		block.setData(bitfield);
		// flag sector as having modified blocks
		s->contents = Sector::CONT_SAVEDATA;
		
		if (id == _CHEST)
		{
			//int index = CreateSectorData(s, bx, by, bz, id);
			//If index <> -1 Then // set index
			//	b->b(bx, bz, by).datapos = index
			//EndIf
		}
		
		if (immediate)
		{
			precompq.addTruckload(*s);
		}
		else
		{
			s->progress = Sector::PROG_NEEDRECOMP;
		}
		// write sector to disk
		chunks.addSector(*s);
		
		if (isLight(id))
		{
			// recount lights for sector (can't be bothered to manage this manually)
			s->countLights();
			// update nearby sectors due to change in light count
			// also, the haslights flag will be RESET for all neighboring sectors to this
			torchlight.lightSectorUpdates(*s, immediate);
		}
		else
		{
			// update nearby sectors only if we are at certain edges
			updateSurroundings(*s, bx, by, bz, immediate);
		}
		
		if (isFluid(id))
		{
			block.setSpecial(1); // reset fluid counter
			int FIXME_work_waterfill_testing;
			//WaterFillTesting(s, bx, by, bz, id, 1)
		}
		else
		{
			// check if adding this block causes water flood
			/*
			if waterfillalgo(s, bx, by, bz) = FALSE then
				'check if closed off a source of water
				if IsFluid(getblock(s, bx, by-1, bz)) then
					WaterCutoff(s, bx, by-1, bz)
				endif
			endif
			*/
		}
		
		// update shadows on nearby sectors by following sun trajectory
		skylightReachDown(*s);
		
		#ifdef FALLING_BLOCKS
			If IsFallingBlock(id) Then
				FallingBlockTesting(s, bx, by, bz, 16)
			EndIf
		#endif
		
		return true;
	}
	
	Block Spiders::removeBlock(int bx, int by, int bz, bool immediate)
	{
		Sector* s = spiderwrap(bx, by, bz);
		// if the given position is outside the local area, null will be returned
		if (s == nullptr) return air_block;
		
		// if the sector is a known nullsector, do absolutely nothing
		if (s->contents == Sector::CONT_NULLSECTOR) return air_block;
		
		if (s->progress == Sector::PROG_NEEDGEN)
		{
			// if the sector needed gen, generate immediately
			Generator::generate(*s, nullptr, 0);
			
			// AGAIN, if the *new* generated sector is a nullsector ...
			if (s->contents == Sector::CONT_NULLSECTOR) return air_block; // do nada
		}
		
		// make a copy of the block, so we can return it
		Block block = s[0](bx, by, bz);
		
		// we can't directly remove fluids or air
		if (Block::fluidToAir(block.getID()) == _AIR) return air_block;
		
		// set the block to _AIR
		s[0](bx, by, bz).setID(_AIR);
		// we removed a non-air block, so decrease block counter
		s->blockpt->blocks -= 1;
		
		// BEFORE we clear the sector entirely, make sure conditions are ok
		s->blockpt->hardsolid = 0; // mui importante! must optimize later
		s->culled = false;         // remove culled flag!
		
		// don't render something with 0 blocks
		if (s->blockpt->blocks == 0)
		{
			s->clear();
			// we need to disable rendering columns that have no blocks anymore
			int FIXME_disable_columns_without_blocks;
			//checkColumn(*s);
		}
		else // sector still has blocks
		{
			// re-render this sector
			if (immediate)
			{
				// add all sectors in column to queue, speeding up recreation
				precompq.addTruckload(*s);
			}
			else s->progress = Sector::PROG_NEEDRECOMP;
		}
		// NOTE: after this point, the sector may be cleared! don't assume blockpt exists!
		
		// write updated sector to disk
		chunks.addSector(*s);
		
		/*
		If b.id = _TNT Then
			BlockExplode(s, bx, by, bz)
			return b
		ElseIf b.id = _CHEST Then
			'RemoveSpecialData(s, id, s->blockpt->b(bx,bz,by).datapos)
			's->blockpt->b(bx,bz,by).datapos = 0 'reset index
		EndIf
		*/
		
		// update neighboring sectors (depending on edges)
		updateSurroundings(*s, bx, by, bz, immediate);
		
		//If WaterFillAlgo(s, bx, by, bz) = FALSE Then
		//	FallingBlockAlgo(s, bx, by, bz)
		//EndIf
		if (isLight(block.getID()))
		{
			s->countLights();
			torchlight.lightSectorUpdates(*s, immediate);
		}
		
		// -- ubercheck if something below could need recompiling
		skylightReachDown(*s);
		// -- end ubercheck
		
		// return COPY of block
		return block;
	}
	
	void Spiders::skylightReachDown(Sector& sector)
	{
		for (int y = 0; y < sector.y; y++)
		{
			if (Sectors(sector.x, y, sector.z).render)
			{
				Sectors(sector.x, y, sector.z).progress = Sector::PROG_NEEDRECOMP;
			}
		}
	}
	
	inline void updateNeighboringSector(Sector& sector, bool immediate)
	{
		if (sector.contents != Sector::CONT_NULLSECTOR)
		{
			sector.progress = Sector::PROG_NEEDRECOMP;
			sector.culled   = false;
			if (immediate) precompq.addTruckload(sector);
		}
	}
	
	void Spiders::updateSurroundings(Sector& sector, int bx, int by, int bz, bool immediate)
	{
		if (bx == 0)
		{
			if (sector.x)
			{
				Sector& testsector = Sectors(sector.x-1, sector.y, sector.z);
				updateNeighboringSector(testsector, immediate);
			}
		}
		else if (bx == Sector::BLOCKS_XZ-1)
		{
			if (sector.x+1 != Sectors.getXZ())
			{
				Sector& testsector = Sectors(sector.x+1, sector.y, sector.z);
				updateNeighboringSector(testsector, immediate);
			}
		}
		if (by == 0)
		{
			if (sector.y)
			{
				Sector& testsector = Sectors(sector.x, sector.y-1, sector.z);
				updateNeighboringSector(testsector, immediate);
			}
		}
		else if (by == Sector::BLOCKS_Y-1)
		{
			if (sector.y+1 != Sectors.getY())
			{
				Sector& testsector = Sectors(sector.x, sector.y+1, sector.z);
				updateNeighboringSector(testsector, immediate);
			}
		}
		if (bz == 0)
		{
			if (sector.z)
			{
				Sector& testsector = Sectors(sector.x, sector.y, sector.z-1);
				updateNeighboringSector(testsector, immediate);
			}
		}
		else if (bz == Sector::BLOCKS_XZ-1)
		{
			if (sector.z+1 != Sectors.getXZ())
			{
				Sector& testsector = Sectors(sector.x, sector.y, sector.z+1);
				updateNeighboringSector(testsector, immediate);
			}
		}
	}
}
