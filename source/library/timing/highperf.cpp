#include "highperf.hpp"

#include <windows.h>

namespace library
{
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
}
