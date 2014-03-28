#include "worldmanager.hpp"

#include "chunks.hpp"
#include "items.hpp"
#include "lighting.hpp"
#include "menu.hpp"
#include "particles.hpp"
#include "player.hpp"
#include "precompq.hpp"
#include "soundman.hpp"
#include "world.hpp"
#include "worldbuilder.hpp"

namespace cppcraft
{
	void WorldManager::init(gamestate_t gs, library::WindowClass& gameScreen, std::string& worldFolder)
	{
		this->gamestate = gs;
		// initalize world
		world.init(worldFolder);
		// initialize player
		player.initPlayer();
		// load world data (if any)
		world.load();
		// initialize chunk systems
		chunks.initChunks();
		// initialize precompiler systems
		precompq.init();
		// initialize lighting
		Lighting.init();
		// initialize world builder
		worldbuilder.init();
		
		// initialize particles
		particleSystem.init();
		
		// menu & items
		menu.init();
		
		// initialize sound
		soundman.init();
		
		// initialize keyboard / joystick input
		player.initInputs(gameScreen);
	}
	
}