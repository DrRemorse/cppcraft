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
	
	void PrecompScheduler::add(const Sector& sector)
	{
		// add to this scheduler
		int cy = sector.y / Columns::COLUMNS_SIZE;
		
		for (std::size_t i = 0; i < psched.size(); i++)
		{
			int sy = psched[i].sector->y / Columns::COLUMNS_SIZE;
			
			// we have to compare against sectors here, because the world is constantly changing
			if (psched[i].sector->x == sector.x && sy == cy && psched[i].sector->z == sector.z)
			{
				// column already exists in scheduler, exit immediately
				return;
			}
		}
		
		psched.emplace_back(sector);
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
		int start_y = sector->y - (sector->y & (Columns::COLUMNS_SIZE-1));
		bool ready = true;
		
		Column& cv = columns(sector->x, sector->y / Columns::COLUMNS_SIZE, sector->z);
		
		for (int sy = 0; sy < Columns::COLUMNS_SIZE; sy++)
		{
			Sector& s2 = Sectors(sector->x, start_y + sy, sector->z);
			
			if (s2.progress != Sector::PROG_COMPILED)
			{
				// avoid resetting the sector compiled status
				ready = false;
			}
			else if (s2.render)
			{
				// a renderable sector without VBO data, is not renderable!
				if (cv.vbodata[sy].pcdata == nullptr)
				{
					logger << Log::ERR << "PrecompSchedule::schedule(): vertex data was null" << Log::ENDL;
					// FIXME this is a bug, somewhere
					s2.progress = Sector::PROG_NEEDRECOMP;
					ready = false;
				}
			}
			else
			{
				if (cv.vbodata[sy].pcdata)
				{
					delete[] cv.vbodata[sy].pcdata;
					cv.vbodata[sy].pcdata = nullptr;
				}
			}
		}
		
		if (ready == false) return false;
		
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
