#ifndef LIBRARY_TIMER_HPP
#define LIBRARY_TIMER_HPP

#include "highperf.hpp"

namespace library
{
	class Timer
	{
	public:
		typedef double time_t;
		
		//! \brief constructor sets t0 to the current time
		Timer();
		
		//! \brief returns the time (in fractional seconds) between the last getDeltaTime()
		//! function call, or time since object was instantiated (whichever happened last)
		time_t getTime();
		
		//! \brief returns time elapsed, and sets new time, so 
		//! all future rounds and deltas are calculated from new time point
		time_t restart();
		
	private:
		HighResClock::time_point t0;
	};
}

#endif
