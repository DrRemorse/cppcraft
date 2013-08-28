#include "precompq.hpp"

#include "library/log.hpp"
#include "library/timing/timer.hpp"
#include "library/threading/thread_pool.hpp"
#include "columns.hpp"
#include "precompiler.hpp"
#include "precomp_thread.hpp"
#include "sectors.hpp"

using namespace library;

namespace cppcraft
{
	PrecompQ precompq;
	ThreadPool* threadpool;
	
	const double PRECOMPQ_MAX_THREADWAIT = 0.025;
	
	void PrecompQ::init()
	{
		// initialize precompiler backend
		precompiler.init();
		// create X amount of threads
		std::size_t threads = precompiler.getThreads();
		// create dormant thread pool
		threadpool = new ThreadPool(threads);
	}
	
	// stop precompq
	void PrecompQ::stop()
	{
		delete threadpool;
	}
	
	void PrecompQ::addTruckload(Sector& s)
	{
		// adds all sectors in this sectors "column" to the queue
		int start_y = s.y - (s.y & (Columns.COLUMNS_SIZE-1));
		int end_y   = start_y + Columns.COLUMNS_SIZE;
		
		// put truckload of sectors into queue
		for (int y = start_y; y < end_y; y++)
		{
			Sector& s2 = Sectors(s.x, y, s.z);
			
			if ( (s2.contents == Sector::CONT_SAVEDATA && s2.vbodata == nullptr) || &s2 == &s )
			{
				addPrecomp(s2);
			}
		} // y
		
	} // addTruckload
	
	bool PrecompQ::addPrecomp(Sector& s)
	{
		// check if this sector is in some pipeline stage
		if (s.precomp)
		{
			// if the sector is currently running, just flag it for recompilation
			if (s.precomp == 2) s.progress = Sector::PROG_NEEDRECOMP;
			
			// since something is going on, exit
			return true;
		}
		// check if sector needs to be generated
		if (s.progress == Sector::PROG_NEEDGEN)
		{
			logger << Log::WARN << "PrecompQ::addPrecomp(): Sector needed generation" << Log::ENDL;
			// not much to do, so exit
			return true;
		}
		
		// because of addTruckload() dependency, some precomps could be alive still
		while(1)
		{
			if (queueCount >= Precompiler::MAX_PRECOMPQ)
			{
				s.progress = Sector::PROG_NEEDRECOMP;
				return false;
			}
			// is the current queueing precomp alive?
			if (precompiler[queueCount].alive == false)
			{
				// if not, break out of loop
				break;
			}
			// otherwise, since it's alive, go to the next precomp in queue
			queueCount += 1;
		}
		
		// if we are here, we have found a precomp that wasn't in-use already
		// add the sector to precompilation queue
		s.progress = Sector::PROG_RECOMP;
		s.precomp  = 1;
		precompiler[queueCount].sector = &s;
		precompiler[queueCount].alive  = true;
		// automatically go to next precomp
		queueCount += 1;
		return (queueCount < Precompiler::MAX_PRECOMPQ);
	}
	
	void precompilerJob(void* pthread)
	{
		PrecompThread& pt = *(PrecompThread*)pthread;
		pt.precompile();
	}
	
	void PrecompQ::finish()
	{
		// clean out older jobs
		if (futures.size())
		{
			threadpool->finishQueue(futures);
			futures.clear();
		}
	}
	
	bool PrecompQ::run(Timer& timer, double localTime)
	{
		// nothing to do, if nothing was added to queue this round
		if (queueCount == 0) return false;
		// current thread id
		int t_mod = 0;
		
		// ------------ PRECOMPILER -------------- //
		
		// reset if out of bounds
		if (currentPrecomp >= Precompiler::MAX_PRECOMPQ)
			currentPrecomp = 0;
		
		while (currentPrecomp < Precompiler::MAX_PRECOMPQ)
		{
			if (precompiler[currentPrecomp].alive)
			{
				Sector* sector = precompiler[currentPrecomp].sector;
				
				if (sector->precomp == 1)
				{
					sector->precomp = 2;
					
					// set thread job info
					PrecompThread& pt = precompiler.getThread(t_mod);
					pt.precomp = &precompiler[currentPrecomp];
					
					// queue thread job
					futures.emplace_back
					(
						threadpool->startJob(precompilerJob, &pt)
					);
					
					t_mod = (t_mod + 1) % precompiler.getThreads();
					if (t_mod == 0)
					{
						finish();
						
						// check if we need to bail out
						if (timer.getDeltaTime() > localTime + PRECOMPQ_MAX_THREADWAIT)
						{
							// increase count so we can get out now
							currentPrecomp += 1;
							// exit
							return true;
						}
					}
					
				}
				//else if (precompiler[currentPrecomp].sector->precomp == 2)
				//{
				//	// sectorcompiler is working on this (hopefully)
				//}
				else if (precompiler[currentPrecomp].sector->precomp == 0)
				{
					// this sector has been reset, probably by seamless()
					// so, just disable it
					precompiler[currentPrecomp].alive = false;
				}
				
			} // if precomp is alive
			
			// count until current max is hit, then exit (and reset)
			currentPrecomp += 1;
			
		} // for each precomp
		
		// reset counters
		queueCount = 0;
		currentPrecomp = 0;
		
		// finish jobs & clear queue
		finish();
		
		// always check if time is out
		if (timer.getDeltaTime() > localTime + PRECOMPQ_MAX_THREADWAIT) return true;
		
		return false;
	}
}
