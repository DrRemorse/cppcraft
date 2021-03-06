#ifndef WORLDBUILDER_HPP
#define WORLDBUILDER_HPP

#include "sector.hpp"

namespace library
{
	class Timer;
}

namespace cppcraft
{
	class Sector;
	
	class WorldBuilder
	{
	public:
		// initializer
		void init();
		// executed when seamless() has transitioned the world
		void seamResponse();
		// executed when a world builder reset is needed
		void reset();
		
		// executed at regular intervals, generating and adding
		// sectors to precompiler queue
		// returns true if worldbuilder exit early due to time exceeded
		bool run(library::Timer& timer, double localTime);
		
	private:
		int diagonal;      // diagonal of outward spiral
		int lastPosition;  // last side of spiral
		int mode;          // worldbuilder mode
		
	};
	extern WorldBuilder worldbuilder;
}

#endif
