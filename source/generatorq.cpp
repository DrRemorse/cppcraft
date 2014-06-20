#include "generatorq.hpp"

#include "generator.hpp"
#include "sector.hpp"

using namespace library;

namespace cppcraft
{
	GeneratorQueue generatorQueue;
	
	void GeneratorQueue::run()
	{
		while (genq.empty() == false)
		{
			Sector* sector = genq.front();
			genq.pop_front();
			
			if (sector->progress == Sector::PROG_NEEDGEN)
			{
				Generator::generate(*sector, nullptr, 0.0);
			}
		}
	}
}

