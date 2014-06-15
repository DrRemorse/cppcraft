#include "generatorq.hpp"

#include <library/timing/timer.hpp>
#include "generator.hpp"
#include "sector.hpp"

using namespace library;

namespace cppcraft
{
	GeneratorQueue generatorQueue;
	
	bool GeneratorQueue::run(Timer& timer, double timeOut)
	{
		while (genq.empty() == false)
		{
			Sector* sector = genq.front();
			genq.pop_front();
			
			if (sector->progress == Sector::PROG_NEEDGEN)
			{
				if (Generator::generate(*sector, &timer, timeOut))
					return true;
			}
		}
		return false;
	}
}

