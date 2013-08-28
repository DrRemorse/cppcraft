#ifndef WORLDMANAGER_HPP
#define WORLDMANAGER_HPP

#include <string>

namespace cppcraft
{
	class Chunks;
	class PrecompQ;
	
	class WorldManager
	{
	public:
		typedef enum
		{
			GS_INIT,
			GS_INTRO,
			GS_TRANSITION,
			GS_RUNNING,
			GS_PAUSED,
			GS_DEATH,
			
			GS_EXIT
			
		} gamestate_t;
		
		// initializer
		void init(gamestate_t, std::string& folder);
		gamestate_t getState();
		// running thread function
		void main();
		
	private:
		gamestate_t gamestate;
	};
	
}

#endif
