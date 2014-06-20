#ifndef GENERATORQ_HPP
#define GENERATORQ_HPP

#include <deque>

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
		void run();
		
	private:
		std::deque<Sector*> genq;
	};
	extern GeneratorQueue generatorQueue;
}

#endif // GENERATORQ_HPP
