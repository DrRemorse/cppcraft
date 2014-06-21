#include "precompq.hpp"

#include <library/log.hpp>
#include <library/config.hpp>
#include <library/timing/timer.hpp>
#include <library/threading/TThreadPool.hpp>
#include "columns.hpp"
#include "precompiler.hpp"
#include "precomp_thread.hpp"
#include "precompq_schedule.hpp"
#include "sectors.hpp"
#include "worldbuilder.hpp"
#include <mutex>
//#define DEBUG

using namespace library;

namespace cppcraft
{
	PrecompQ precompq;
	ThreadPool::TPool* threadpool;
	std::mutex jobsynch;
	
	class PrecompJob : public ThreadPool::TPool::TJob
	{
	public:
		PrecompJob (int p) : ThreadPool::TPool::TJob(p)
		{
			this->pt = new PrecompThread();
			precomp = nullptr;
			is_done = true;
		}
		
		void prepareJob()
		{
			is_done  = false;
		}
		void run (void* _precomp)
		{
			this->precomp = (Precomp*) _precomp;
			
			if (precomp->getJob() == Sector::PROG_RECOMPILING)
			{
				// first precompiler stage: mesh generation
				pt->precompile(*precomp);
			}
			else if (precomp->getJob() == Sector::PROG_AO)
			{
				// second stage: AO
				pt->ambientOcclusion(*precomp);
			}
			else
			{
				logger << Log::WARN << "PrecompJob(): Unknown job: " << precomp->getJob() << Log::ENDL;
				precomp->result = Precomp::STATUS_FAILED;
			}
			
			jobsynch.lock();
			this->is_done = true;
			jobsynch.unlock();
		}
		
		bool isDone()
		{
			jobsynch.lock();
			bool result = is_done;
			jobsynch.unlock();
			return result;
		}
		Precomp* getPrecomp()
		{
			return precomp;
		}
		
	private:
		PrecompThread* pt;
		Precomp* precomp;
		bool     is_done;
	};
	std::vector<PrecompJob> jobs;
	
	void PrecompQ::init()
	{
		// initialize precompiler backend
		precompiler.init();
		
		// create dormant thread pool
		this->threads = config.get("world.threads", 2);
		threadpool = new ThreadPool::TPool(this->threads);
		
		// create jobs
		int jobCount = config.get("world.jobs", 2);
		for (int i = 0; i < jobCount; i++)
			jobs.emplace_back(i);
		
		// next job is first job
		this->nextJobID = 0;
	}
	// stop precompq
	void PrecompQ::stop()
	{
		delete threadpool;
	}
	
	
	bool PrecompQ::startJob(Precomp& precomp)
	{
		if (jobs[this->nextJobID].isDone() == false) return true;
		
		// complete any previously running job
		Precomp* prev = jobs[this->nextJobID].getPrecomp();
		if (prev) checkJobStatus(*prev);
		
		// execute new job
		Sector& sector = *precomp.sector;
		if (sector.progress == Sector::PROG_RECOMPILE)
		{
			sector.progress = Sector::PROG_RECOMPILING;
		}
		else if (sector.progress == Sector::PROG_NEEDAO)
		{
			sector.progress = Sector::PROG_AO;
		}
		else
		{
			// an error hath occured
			logger << Log::ERR << "PrecompQ::startJob(): Unknown progress (" << (int)sector.progress << ")" << Log::ENDL;
			return false;
		}
		
		precomp.result = Precomp::STATUS_NEW;
		precomp.job    = sector.progress;
		jobs[this->nextJobID].prepareJob();
		
		// schedule job
		threadpool->run(&jobs[this->nextJobID], &precomp, false);
		
		// go to next job
		this->nextJobID = (this->nextJobID + 1) % jobs.size();
		// return good news
		return (this->nextJobID == 0);
	}
	
	void PrecompQ::finish()
	{
		threadpool->sync_all();
		this->nextJobID = 0;
	}
	
	// before we can use this job index, we need to check if there are any results
	// from completing a previous job on this index (there are a limited number of jobs)
	void PrecompQ::checkJobStatus(Precomp& precomp)
	{
		if (precomp.alive)
		{
			Precomp::jobresult_t result = precomp.getResult();
			
			if (result == Precomp::STATUS_NEW)
			{
				// this precomp is ready to go, or waiting to be replaced
				return;
			}
			else if (result == Precomp::STATUS_FAILED)
			{
				logger << Log::WARN << "PrecompQ(): Job returned failure" << Log::ENDL;
				Sector& sector = *precomp.sector;
				sector.culled = true;
				sector.render = false;
				sector.progress = Sector::PROG_COMPILED;
				precomp.alive = false;
			}
			else if (result == Precomp::STATUS_CULLED)
			{
				Sector& sector = *precomp.sector;
				if (sector.progress > Sector::PROG_NEEDRECOMP)
				{
					sector.culled = true;
					sector.render = false;
					sector.progress = Sector::PROG_COMPILED;
					precomp.alive = false;
				}
			}
			else if (result == Precomp::STATUS_DONE)
			{
				Sector& sector = *precomp.sector;
				if (sector.progress > Sector::PROG_RECOMPILE)
					sector.progress++;
				//logger << Log::INFO << "Sector progress: " << (int)sector.progress << Log::ENDL;
			}
			else
			{
				// blast the logs with an error
				logger << Log::ERR << "Precomp(): Job running? result = " << result << Log::ENDL;
				return;
			}
		}
		// reset result, since we no longer want to change status
		precomp.result = Precomp::STATUS_NEW;
	}
	
	bool PrecompQ::run(Timer& timer, double timeOut)
	{
		/// ------------ PRECOMPILER ------------ ///
		bool everythingDead = true;
		bool nomorejobs = false;
		
		// check for any finished jobs
		for (PrecompJob& job : jobs)
		{
			// complete any previously running job
			if (job.isDone())
			if (job.getPrecomp())
			{
				checkJobStatus(*job.getPrecomp());
			}
		}
		
		for (int i = 0; i < Precompiler::MAX_PRECOMPQ; i++)
		{
			if (precompiler[i].alive)
			{
				Sector& sector = *precompiler[i].sector;
				
				if (sector.progress == Sector::PROG_RECOMPILE)
				{
					if (precompiler[i].isolator())
					{
						// start job
						if (nomorejobs == false)
						if (startJob(precompiler[i])) nomorejobs = true;
					}
				}
				else if (sector.progress == Sector::PROG_NEEDAO)
				{
					// start job
					if (nomorejobs == false)
					if (startJob(precompiler[i])) nomorejobs = true;
				}
				else if (sector.progress == Sector::PROG_RECOMPILING || sector.progress == Sector::PROG_AO)
				{
					// being worked on by precompiler / ao
				}
				else if (sector.progress == Sector::PROG_NEEDCOMPILE)
				{
					// ready for completion
					// verify that this sector can be assembled into a column properly
					precompiler[i].complete();
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
		
		if (everythingDead)
		{
			// reset counters
			queueCount = 0;
		}
		
		// always check if time is out
		if (timer.getDeltaTime() > timeOut) return true;
		
		// handle transition from this thread to rendering thread
		// from precomp scheduler to compiler scheduler
		// also re-creates missing content making sure things aren't put on the backburner
		PrecompScheduler::scheduling();
		
		// always check if time is out
		if (timer.getDeltaTime() > timeOut) return true;
		
		return false;
	}
}
