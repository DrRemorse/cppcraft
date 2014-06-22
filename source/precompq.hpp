#ifndef PRECOMPQ_HPP
#define PRECOMPQ_HPP

namespace library
{
	class Timer;
}

namespace cppcraft
{
	class Sector;
	class Precomp;
	
	class PrecompQ
	{
	public:
		//! adds a specific @sector to the precompiler queue
		bool addPrecomp(Sector& sector);
		//! adds a complete column of sectors from the column containing @sector
		void addTruckload(Sector& sector);
		
		//! initializes threadpool and creates N jobs
		void init();
		//! returns true if we must add stuff to precompq
		inline bool ready() const
		{
			return queueCount == 0;
		}
		//! finish any remaining queue
		void finish();
		//! stops threadpool
		void stop();
		
		// executes one round of precompilation
		// very time consuming, running N threads in parallell and waits for them to finish
		bool run(library::Timer& timer, double timeOut);
		
	private:
		// starting a job is actually a little complicated
		bool startJob(Precomp& precomp);
		// check job status...
		void checkJobStatus(Precomp& precomp);
		
		int precompIndex(Sector& sector) const;
		int newPrecompIndex() const;
		
		// the number of precomps that was added to queue
		int queueCount = 0;
		// number of parallell threads
		int threads;
		// next job ID
		int nextJobID;
	};
	extern PrecompQ precompq;
}

#endif
