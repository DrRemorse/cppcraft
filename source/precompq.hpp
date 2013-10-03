#ifndef PRECOMPQ_HPP
#define PRECOMPQ_HPP

#include <future>
#include <vector>

namespace library
{
	class Timer;
}

namespace cppcraft
{
	class Sector;
	
	class PrecompQ
	{
	public:
		// adds a specific column to precompiler queue
		bool addPrecomp(Sector& s);
		// adds a complete column of sectors from the column "containing" s
		void addTruckload(Sector& s);
		
		// initializes threading pool
		void init();
		// starting a job is actually a little complicated
		bool startJob(int& t_mod, int job, int stage);
		// finish any remaining queue
		void finish();
		// stop threading pool
		void stop();
		
		// executes one round of precompilation
		// very time consuming, running N threads in parallell and waits for them to finish
		bool run(library::Timer& timer, double localTime);
		
	private:
		// the number of precomps that was added to queue
		int queueCount   = 0;
		// the current (or last) precomp being precompiled
		int currentPrecomp = 0;
		// number of parallell threads
		int threads;
	};
	extern PrecompQ precompq;
}

#endif
