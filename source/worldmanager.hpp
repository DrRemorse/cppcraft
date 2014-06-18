#ifndef WORLDMANAGER_HPP
#define WORLDMANAGER_HPP

#include <string>

namespace library
{
	class WindowClass;
}

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
		void init(gamestate_t, library::WindowClass& scr, std::string& folder);
		void initPlayer();
		// running thread function
		void main();
		
		void teleportHandler();
		
		gamestate_t getState() const noexcept
		{
			return this->gamestate;
		}
		
	private:
		gamestate_t gamestate;
	};
	
}

#endif
