#include "precompq.hpp"

#include "library/log.hpp"
#include "library/timing/timer.hpp"
#include "library/threading/TThreadPool.hpp"
#include "columns.hpp"
#include "precompiler.hpp"
#include "precomp_thread.hpp"
#include "sectors.hpp"
#include "worldbuilder.hpp"

using namespace library;

namespace cppcraft
{
	PrecompQ precompq;
	ThreadPool::TPool* threadpool;
	
	class PrecompJob : public ThreadPool::TPool::TJob
	{
	public:
		PrecompJob (int p) : ThreadPool::TPool::TJob(p) {}
		
		void run (void* pthread)
		{
			PrecompThread& pt = *(PrecompThread*)pthread;
			Sector& sector = *pt.precomp->sector;
			
			if (sector.progress == Sector::PROG_RECOMPILING)
			{
				// first precompiler stage: mesh generation
				pt.precompile();
			}
			else if (sector.progress == Sector::PROG_NEEDAO)
			{
				// second stage: AO
				sector.progress = Sector::PROG_AO;
				pt.ambientOcclusion();
			}
			else
			{
				logger << "starting unknown job, stage = " << (int) sector.progress << Log::ENDL;
			}
		}
	};
	std::vector<PrecompJob*> jobs;
	
	const double PRECOMPQ_MAX_THREADWAIT = 0.0125;
	
	void PrecompQ::init()
	{
		// initialize precompiler backend
		precompiler.init();
		// create X amount of threads
		this->threads = precompiler.getThreads();
		// create dormant thread pool
		threadpool = new ThreadPool::TPool(this->threads);
		// create fixed amount of jobs
		for (int i = 0; i < threads; i++)
			jobs.push_back(new PrecompJob(i));
	}
	
	// stop precompq
	void PrecompQ::stop()
	{
		delete threadpool;
	}
	
	void PrecompQ::addTruckload(Sector& s)
	{
		// set sector recompilation flag immediately
		s.progress = Sector::PROG_NEEDRECOMP;
		
		// adds all sectors in this sectors "column" to the queue
		int start_y = s.y - (s.y & (Columns.COLUMNS_SIZE-1));
		int end_y   = start_y + Columns.COLUMNS_SIZE;
		
		// put truckload of sectors into queue
		for (int y = start_y; y < end_y; y++)
		{
			Sector& s2 = Sectors(s.x, y, s.z);
			
			s2.progress = Sector::PROG_NEEDRECOMP;
			
			// try to add all sectors that need recompilation, until queue is full
			if (s2.progress == Sector::PROG_NEEDRECOMP)
			{
				if (addPrecomp(s2) == false) return;
			}
			
		} // y
		
		// tell worldbuilder to immediately start over, UNLESS its generating
		worldbuilder.reset();
		
	} // addTruckload
	
	bool PrecompQ::addPrecomp(Sector& s)
	{
		// check if this sector is in some pipeline stage
		if (s.progress > Sector::PROG_NEEDRECOMP && s.progress != Sector::PROG_COMPILED)
		{
			// just flag it for (later) recompilation
			s.progress = Sector::PROG_NEEDRECOMP;
			// NOTE: returning false here can make the queue become clogged up
			return true;
		}
		
		// check if sector needs to be generated
		if (s.progress == Sector::PROG_NEEDGEN)
		{
			logger << Log::WARN << "PrecompQ::addPrecomp(): Sector needed generation" << Log::ENDL;
			// not much to do, so exit
			return true;
		}
		
		// check that the sector isn't already in the queue
		for (int i = 0; i < Precompiler::MAX_PRECOMPQ; i++)
		{
			if (precompiler[i].alive)
			{
				if (precompiler[i].sector == &s)
				{
					//logger << Log::WARN << "Sector was in (live) pc queue already" << Log::ENDL;
					
					// if the sector was already in the queue, just start precompiling right this instant!
					s.progress = Sector::PROG_RECOMPILE;
					return false;
					
				}
			}
		}
		
		// because of addTruckload() dependency, some precomps could be alive still
		while (true)
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
		s.progress = Sector::PROG_RECOMPILE;
		precompiler[queueCount].sector = &s;
		precompiler[queueCount].alive  = true;
		// automatically go to next precomp
		queueCount += 1;
		return (queueCount < Precompiler::MAX_PRECOMPQ);
	}
	
	bool PrecompQ::startJob(int& t_mod, int job, int stage)
	{
		// set thread job info
		PrecompThread& pt = precompiler.getThread(t_mod);
		pt.precomp = &precompiler[job];
		
		bool cont = true;
		
		if (stage == Sector::PROG_RECOMPILE)
		{
			// before starting precompiler we need to isolate all data properly
			// then check if the precomp can actually be run
			cont = pt.isolator();
		}
		
		if (cont)
		{
			// queue thread job
			threadpool->run(jobs[t_mod], &pt, false);
			
			// go to next thread
			t_mod = (t_mod + 1) % precompiler.getThreads();
			
			// if we are back at the start, we may just be exiting
			if (t_mod == 0) return true;
		}
		return false;
	}
	
	void PrecompQ::finish()
	{
		threadpool->sync_all();
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
				
				if (sector->progress == Sector::PROG_RECOMPILE || sector->progress == Sector::PROG_NEEDAO)
				{
					if (startJob(t_mod, currentPrecomp, sector->progress))
					{
						finish();
						
						// increase count so we can get out now
						currentPrecomp += 1;
						// exit for
						break;
					}
				}
				else if (sector->progress == Sector::PROG_RECOMPILING || sector->progress == Sector::PROG_AO)
				{
					// being worked on by precompiler / ao
				}
				else if (sector->progress == Sector::PROG_NEEDCOMPILE)
				{
					// complete the transaction
					// verify that this sector can be assembled into a column properly
					precompiler[currentPrecomp].complete();
				}
				else  // if (precompiler[currentPrecomp].sector->precomp == 0)
				{
					// this sector has been reset, probably by seamless()
					// so, just disable it
					precompiler[currentPrecomp].alive = false;
				}
				
			} // if precomp is alive
			
			// count until current max is hit, then exit (and reset)
			currentPrecomp += 1;
			
		} // for each precomp
		
		if (currentPrecomp >= Precompiler::MAX_PRECOMPQ)
		{
			// reset counters
			queueCount = 0;
			currentPrecomp = 0;
			
			if (t_mod)
			{
				// finish jobs & clear queue
				finish();
			}
		}
		
		// always check if time is out
		if (timer.getDeltaTime() > localTime + PRECOMPQ_MAX_THREADWAIT) return true;
		
		return false;
	}
}
