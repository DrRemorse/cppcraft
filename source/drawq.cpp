#include "drawq.hpp"

#include "library/log.hpp"
#include "sectors.hpp"
#include <string>

using namespace library;

namespace cppcraft
{
	DrawQueue drawq;
	DrawQueue reflectionq;
	
	// create all queue shaderlines
	void DrawQueue::init()
	{
		logger << Log::INFO << "* Initializing rendering queue" << Log::ENDL;
		
		// initialize each queue
		reset();
	}
	
	// reset all shaderlines
	void DrawQueue::reset()
	{
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			this[0][i].clear();
		}
	}
	
}
