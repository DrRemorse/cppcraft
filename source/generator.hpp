/**
 * Generator
 * 
 * Chunk loader, loading compressed / raw chunks
 * from world file system
 * 
**/

#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "sector.hpp"

namespace library
{
	class Timer;
}

namespace cppcraft
{
	class Sector;
	
	class Generator
	{
	public:
		static void init();
		static bool generate(Sector& sector, library::Timer* timer, double timeOut);
		
	private:
		static void loadSector(Sector&, std::ifstream&, unsigned int);
		
	};
}

#endif
