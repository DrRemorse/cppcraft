#include "worldmanager.hpp"

#include "chunks.hpp"
#include "flatlands.hpp"
#include "items.hpp"
#include "lighting.hpp"
#include "menu.hpp"
#include "particles.hpp"
#include "player.hpp"
#include "precompq.hpp"
#include "sectors.hpp"
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
	
	void WorldManager::initPlayer()
	{
		// center grid, center sector, center block
		player.X = ((float)Sectors.getXZ() / 2.0 - 0.5) * Sector::BLOCKS_XZ + 0.5;
		player.Z = player.X;
		player.Y = Sectors.getY() * Sector::BLOCKS_Y * 0.75;
		// load world data (if any)
		world.load();
	}
}