#include "highperf.hpp"

#ifdef __linux__
	#include <chrono>
#else
	#include <windows.h>
#endif

namespace library
{
	#ifdef __linux__
		HighResClock::time_point HighResClock::now()
		{
			/*
				‘std::chrono::system_clock::time_point
				{aka std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long int, std::ratio<1l, 1000000000l> > >}’
				
				‘library::HighResClock::time_point
				{aka std::chrono::time_point<library::HighResClock>}’
			*/
			return std::chrono::high_resolution_clock::now();
		}
	#else
		const long long g_Frequency = []() -> long long 
		{
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			return frequency.QuadPart;
		}();
		
		HighResClock::time_point HighResClock::now()
		{
			LARGE_INTEGER count;
			QueryPerformanceCounter(&count);
			return time_point(duration(count.QuadPart * static_cast<rep>(period::den) / g_Frequency));
		}
	#endif
}
