#ifndef GENERATORQ_HPP
#define GENERATORQ_HPP

#include <deque>

namespace library
{
	class Timer;
}

namespace cppcraft
{
	class Sector;
	
	class GeneratorQueue
	{
	public:
		inline void add(Sector* sector)
		{
			genq.push_back(sector);
		}
		
		// returns true if time ran out
		bool run(library::Timer& timer, double timeOut);
		
	private:
		std::deque<Sector*> genq;
	};
	extern GeneratorQueue generatorQueue;
}

#endif // GENERATORQ_HPP
