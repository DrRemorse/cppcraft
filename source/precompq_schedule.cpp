#include "precompq_schedule.hpp"

#include <library/log.hpp>
#include "columns.hpp"
#include "compilers.hpp"
#include "precompq.hpp"
#include "sectors.hpp"
#include "threading.hpp"
#include "vertex_block.hpp"
#include <vector>

using namespace library;

namespace cppcraft
{
	std::vector<PrecompSchedule> psched;
	
	// add column to this scheduler
	// the sector reference is really only for the coordinates (x, cy, z)
	void PrecompScheduler::add(const Sector& sector)
	{
		for (size_t i = 0; i < psched.size(); i++)
		{
			// we have to compare against sectors here, because the world is constantly changing
			if (psched[i].sector->getX() == sector.getX() && 
				psched[i].sector->getY() == sector.getY() && 
				psched[i].sector->getZ() == sector.getZ())
			{
				// column already exists in scheduler, exit immediately
				return;
			}
		}
		
		psched.emplace_back(sector);
	}
	
	void PrecompScheduler::reset()
	{
		psched.clear();
	}
	
	template <typename T>
	void remove_at(std::vector<T>&v, typename std::vector<T>::size_type n)
	{
		std::swap(v[n], v.back());
		v.pop_back();
	}
	
	void PrecompScheduler::scheduling()
	{
		for (int i = psched.size()-1; i >= 0; i--)
		{
			if (psched[i].schedule())
			{
				// if the item was passed on to compiler, remove it
				remove_at(psched, i);
			}
		}
	}
	
	bool PrecompSchedule::schedule()
	{
		// determine readiness of column before sending to compiler
		// first sector & end iterator in column
		int start_y = columns.getSectorLevel(sector->getY());
		bool ready = true;
		bool foundData = false;
		
		int x = sector->getX();
		int z = sector->getZ();
		
		Column& cv = columns(x, sector->getY(), z);
		int columnSize = columns.getSizeInSectors(sector->getY());
		
		for (int sy = 0; sy < columnSize; sy++)
		{
			Sector& s2 = Sectors(x, start_y + sy, z);
			
			if (s2.progress != Sector::PROG_COMPILED)
			{
				ready = false;
			}
			else if (s2.render)
			{
				// a renderable sector without VBO data, is not renderable!
				if (cv.vbodata[sy].pcdata == nullptr)
				{
					//logger << Log::ERR << "PrecompSchedule::schedule(): vertex data was null" << Log::ENDL;
					// FIXME this is a bug: Seamless resets column data, and the sectors they come from
					// but the sector might still remain in the scheduling/transfer queue (this) from before
					s2.progress = Sector::PROG_NEEDRECOMP;
					ready = false;
				}
				// yay, at least one sector had data
				else foundData = true;
			}
			else
			{
				// sector mesh-data isn't needed at this time
				if (cv.vbodata[sy].pcdata)
				{
					delete[] cv.vbodata[sy].pcdata;
					cv.vbodata[sy].pcdata = nullptr;
				}
			}
		}
		
		// either the column isn't ready to be assembled
		if (ready == false)
		{
			//mtx.compiler.unlock();
			return false;
		}
		// or, there simply wasn't anything to assemble
		if (foundData == false)
		{
			cv.renderable = false;
			//mtx.compiler.unlock();
			return false;
		}
		
		/// add to compiler queue ///
		mtx.compiler.lock();
		
		if (cv.updated == false)
		{
			cv.updated = true;
			
			// last: add to queue
			compilers.add(sector);
		}
		mtx.compiler.unlock();
		
		return ready;
	}
	
}
