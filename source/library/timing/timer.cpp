#include "timer.hpp"

using namespace std::chrono;

namespace library
{
	Timer::Timer()
	{
		// the starting time, which is updated at each "interval"
		t0 = HighResClock::now();
	}
	
	// returns time elapsed in high-precision seconds
	Timer::time_t Timer::getDeltaTime()
	{
		HighResClock::time_point t1 = HighResClock::now();
		microseconds micros = duration_cast<microseconds>(t1 - t0);
		
		// return high-precision seconds
		return micros.count() / 1e6d;
	}
	
	// defines an interval change, and returns the time between last call
	// in high-precision seconds
	Timer::time_t Timer::startNewRound()
	{
		HighResClock::time_point t1 = HighResClock::now();
		microseconds micros = duration_cast<microseconds>(t1 - t0);
		
		// set last time = this time
		t0 = t1;
		// return high-precision seconds
		return micros.count() / 1e6d;
	}
}