#include "precompq.hpp"

#include <library/log.hpp>
#include <library/timing/timer.hpp>
#include <library/threading/TThreadPool.hpp>
#include "columns.hpp"
#include "precompiler.hpp"
#include "precomp_thread.hpp"
#include "precompq_schedule.hpp"
#include "sectors.hpp"
#include "worldbuilder.hpp"

using namespace library;

namespace cppcraft
{
	PrecompQ precompq;
	ThreadPool::TPool* threadpool;
	static const double PRECOMPQ_MAX_THREADWAIT = 0.013;
	
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
				try
				{
					// first precompiler stage: mesh generation
					pt.precompile();
				}
				catch (std::string exc)
				{
					logger << Log::ERR << "pt.precompile(): " << exc << Log::ENDL;
				}
			}
			else if (sector.progress == Sector::PROG_AO)
			{
				try
				{
					// second stage: AO
					pt.ambientOcclusion();
				}
				catch (std::string exc)
				{
					logger << Log::ERR << "pt.ambientOcclusion(): " << exc << Log::ENDL;
				}
			}
			else
			{
				logger << "PrecompJob(): Unknown job: " << (int) sector.progress << Log::ENDL;
			}
		}
	};
	std::vector<PrecompJob*> jobs;
	
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
	
	// must return index, because it could be less than the queue counter
	// which could make the precompq indexifier go mad MAD I SAY MADDDDDD
	int PrecompQ::precompIndex(Sector& sector) const
	{
		for (int i = 0; i < Precompiler::MAX_PRECOMPQ; i++)
		{
			if (precompiler[i].alive)
			{
				if (precompiler[i].sector == &sector)
				{
					return i;
				}
			}
		}
		return -1;
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
		
		// tell worldbuilder to immediately start over, UNLESS its generating
		// the sectors closest to the player will be rebuilt sooner, rather than later
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
		if (s.progress >= Sector::PROG_RECOMPILE)
		{
			//logger << Log::WARN << "PrecompQ::addPrecomp(): Sector already about to be recompiled" << Log::ENDL;
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
			s.progress = Sector::PROG_RECOMPILE;
			return true;
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
	
	bool PrecompQ::startJob(int& t_mod, int job)
	{
		// set thread job info
		PrecompThread& pt = precompiler.getThread(t_mod);
		pt.precomp = &precompiler[job];
		int stage = ++pt.precomp->sector->progress;
		
		bool cont = true;
		
		if (stage == Sector::PROG_RECOMPILING)
		{
			// before starting precompiler we need to isolate all data properly
			// then check if the precomp can actually be run (or is needed at all)
			cont = pt.isolator();
		}
		
		if (cont)
		{
			// queue thread job
			threadpool->run(jobs[t_mod], &pt, false);
			
			// go to next thread
			t_mod = (t_mod + 1) % this->threads;
			
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
		// current thread id
		int t_mod = 0;
		
		// ------------ PRECOMPILER -------------- //
		
		bool everythingDead = true;
		int i;
		for (i = 0; i < Precompiler::MAX_PRECOMPQ; i++)
		{
			if (precompiler[i].alive)
			{
				Sector& sector = *precompiler[i].sector;
				
				if (sector.progress == Sector::PROG_RECOMPILE || sector.progress == Sector::PROG_NEEDAO)
				{
					if (startJob(t_mod, i))
					{
						finish();
						
						// always check if time is out
						if (timer.getDeltaTime() > localTime + PRECOMPQ_MAX_THREADWAIT)
						{
							everythingDead = false;
							
							i++;
							// exit for
							break;
						}
					}
				}
				else if (sector.progress == Sector::PROG_RECOMPILING || sector.progress == Sector::PROG_AO)
				{
					// being worked on by precompiler / ao
				}
				else if (sector.progress == Sector::PROG_NEEDCOMPILE)
				{
					// ready for completion
					// verify that this sector can be assembled into a column properly
					try
					{
						precompiler[i].complete();
					}
					catch (std::string exc)
					{
						logger << Log::ERR << "Precomp::complete(): " << exc << Log::ENDL;
					}
				}
				else  // if (precompiler[currentPrecomp].sector->precomp == 0)
				{
					// this sector has been reset, probably by seamless()
					// so, just disable it
					precompiler[i].alive = false;
				}
				
				if (precompiler[i].alive) everythingDead = false;
				
			} // if precomp is alive
			
		} // for each precomp
		
		if (t_mod)
		{
			// finish jobs & clear queue
			finish();
		}
		
		if (everythingDead)
		{
			// reset counters
			queueCount = 0;
		}
		
		// handle transition from this thread to rendering thread
		// from precomp scheduler to compiler scheduler
		// also re-creates missing content making sure things aren't put on the backburner
		PrecompScheduler::scheduling();
		
		// always check if time is out
		if (timer.getDeltaTime() > localTime + PRECOMPQ_MAX_THREADWAIT) return true;
		
		return false;
	}
}
