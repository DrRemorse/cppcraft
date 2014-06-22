#include "precompq.hpp"

#include <library/log.hpp>
#include "columns.hpp"
#include "precompiler.hpp"
#include "sectors.hpp"
#include "worldbuilder.hpp"
#include <mutex>
//#define DEBUG

using namespace library;

namespace cppcraft
{
	// must return index, because it could be less than the queue counter
	// which could make the precompq indexifier go mad MAD I SAY MADDDDDD
	int PrecompQ::precompIndex(Sector& sector) const
	{
		for (int i = 0; i < this->queueCount; i++)
		{
			if (precompiler[i].alive)
			if (precompiler[i].sector == &sector)
			{
				return i;
			}
		}
		return -1;
	}
	// returns the index of a new (free) precomp index, or -1 if queue is full
	int PrecompQ::newPrecompIndex() const
	{
		for (int i = 0; i < Precompiler::MAX_PRECOMPQ; i++)
		{
			if (precompiler[i].alive == false) return i;
		}
		return -1;
	}
	
	void PrecompQ::addTruckload(Sector& s)
	{
		// adds all sectors in this sectors "column" to the queue
		int columnY = columns.fromSectorY(s.getY());
		int start_y = columns.getSectorLevel(columnY);
		int end_y   = start_y + columns.getSizeInSectors(columnY);
		
		int x = s.getX();
		int z = s.getZ();
		
		// put truckload of sectors into queue
		for (int y = start_y; y < end_y; y++)
		{
			Sector& s2 = Sectors(x, y, z);
			
			// try to add all sectors that need recompilation, until queue is full
			if (s2.contents == Sector::CONT_SAVEDATA && s2.culled == false)
			{
				addPrecomp(s2);
			}
			
		} // y
		
		// tell worldbuilder to immediately start over
		// the sectors closest to center will now be rebuilt sooner, rather than later
		worldbuilder.reset();
		
	} // addTruckload
	
	bool PrecompQ::addPrecomp(Sector& s)
	{
		// check if sector needs to be generated
		if (s.progress == Sector::PROG_NEEDGEN)
		{
			logger << Log::ERR << "PrecompQ::addPrecomp(): Sector needed generation" << Log::ENDL;
			// not much to do, so exit
			return true;
		}
		
		// check that the sector isn't already in the queue
		int index = precompIndex(s);
		if (index != -1)
		{
			// if the sector was already in the queue, just restart
			s.progress = Sector::PROG_RECOMPILE;
			return true;
		}
		// find a free precomp spot
		index = newPrecompIndex();
		if (index == -1)
		{
			s.progress = Sector::PROG_NEEDRECOMP;
			return false;
		}
		
		// set new queueCount if needed
		if (index <= queueCount) queueCount = index + 1;
		
		// if we are here, we have found a precomp that wasn't in-use already
		// add the sector to precompilation queue
		s.progress = Sector::PROG_RECOMPILE;
		precompiler[index].sector = &s;
		precompiler[index].alive  = true;
		return true;
	}
	
}
