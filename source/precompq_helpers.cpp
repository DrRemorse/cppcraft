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
		for (int i = 0; i < Precompiler::MAX_PRECOMPQ; i++)
		{
			if (precompiler[i].alive)
			if (precompiler[i].sector == &sector)
			{
				return i;
			}
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
			
			// NOTE bug fixed:
			// don't try to re-add sectors that are already in the owen,
			// because different threads might just start to work on the same sector
			
			// try to add all sectors that need recompilation, until queue is full
			if (s2.contents == Sector::CONT_SAVEDATA && s2.culled == false)
			{
				if (s2.progress == Sector::PROG_NEEDRECOMP)
					// we can directly add sector that is flagged as ready
					addPrecomp(s2);
				else if (s2.progress > Sector::PROG_RECOMPILE)
					// we also don't want to REschedule sectors already being compiled
					// that means we really just want to change its flag if its already in the owen
					// NOTE: this will cause this same function to try to add living precomps to queue
					s2.progress = Sector::PROG_NEEDRECOMP;
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
		
		// check if sector is about to be precompiled
		if (s.progress == Sector::PROG_COMPILED)
		{
			logger << Log::WARN << "PrecompQ::addPrecomp(): Sector was compiled" << Log::ENDL;
		}
		else if (s.progress >= Sector::PROG_RECOMPILE)
		{
			logger << Log::WARN << "PrecompQ::addPrecomp(): Sector already about to be recompiled" << Log::ENDL;
			s.progress = Sector::PROG_RECOMPILE;
			// not much to do, so exit
			return true;
		}
		
		// check that the sector isn't already in the queue
		int index = precompIndex(s);
		if (index != -1)
		{
			//logger << Log::INFO << "PrecompQ::addPrecomp(): Already existed " << s.x << ", " << s.y << ", " << s.z << Log::ENDL;
			//logger << Log::INFO << "progress: " << (int) s.progress << Log::ENDL;
			
			// if the sector was already in the queue, just start recompiling right this instant!
			precompiler[index].alive = false;
			s.progress = Sector::PROG_NEEDRECOMP;
			//return true;
		}
		
		// because of addTruckload() dependency, some precomps could be alive still
		while (true)
		{
			// if the queue is full, return immediately
			if (queueCount >= Precompiler::MAX_PRECOMPQ)
				return false;
			
			// is the current precomp queue index not in use?
			if (precompiler[queueCount].alive == false)
			{
				// if its not, break out of loop
				break;
			}
			// otherwise, since it's alive, go to the next index in queue
			queueCount += 1;
		}
		
		// if we are here, we have found a precomp that wasn't in-use already
		// add the sector to precompilation queue
		s.progress = Sector::PROG_RECOMPILE;
		precompiler[queueCount].sector = &s;
		precompiler[queueCount].alive  = true;
		// automatically go to next precomp
		queueCount += 1;
		return (queueCount < Precompiler::MAX_PRECOMPQ);
	}
	
}
