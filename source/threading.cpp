#include "threading.hpp"

#include "library/config.hpp"
#include "worldmanager.hpp"

namespace cppcraft
{
	ThreadingClass mtx;
	
	void ThreadingClass::initThreading(WorldManager& worldman)
	{
		// start world manager thread
		mtx.worldman = std::thread(&WorldManager::main, worldman);
	}
	
	void ThreadingClass::cleanupThreading()
	{
		mtx.terminate = true;
		// wait for worldman thread to end
		mtx.worldman.join();
	}
	
}
