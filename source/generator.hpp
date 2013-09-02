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

namespace cppcraft
{
	class Sector;
	
	class Generator
	{
	public:
		static void init();
		static void generate(Sector& sector);
		
	private:
		static void loadSector(Sector&, std::ifstream&, unsigned int);
		
	};
}

#endif
