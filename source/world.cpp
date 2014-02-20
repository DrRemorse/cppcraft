#include "world.hpp"

#include "library/config.hpp"
#include "sectors.hpp"
#include "flatlands.hpp"
#include "player.hpp"
#include <fstream>

using namespace library;

namespace cppcraft
{
	// default world folder
	static const std::string DEFAULT_WORLD_FOLDER = "world";
	// one and only instance of WorldClass
	World world;
	
	void World::init(std::string& worldFolder)
	{
		/// initialize sectors, blocks & flatlands ///
		
		int sectors_xz = config.get("sectors_axis", 48);
		Sectors.createSectors(sectors_xz);
		// also, initialize 2D flatlands (fsectors)
		flatlands.init();
		
		/// load world position from folder ///
		
		// initialize world coordinates centering player in the world
		worldCoords.worldX = WORLD_STARTING_X - sectors_xz / 2; //+ 84; //
		worldCoords.worldY = 0;
		worldCoords.worldZ = WORLD_STARTING_Z - sectors_xz / 2; //- 104; //
		// finally, world folder, where world-data is located
		if (worldFolder.size() == 0)
		{
			std::string folder = config.get("world", "");
			if (folder.size() == 0)
			{
				this->folder = DEFAULT_WORLD_FOLDER;
			}
			else
			{
				this->folder = folder;
			}
		}
		else
		{
			this->folder = worldFolder;
		}
	}
	
	void World::load()
	{
		std::ifstream ff (worldFolder() + "/world.data", std::ios::in | std::ios::binary);
		if (!ff) return;
		
		ff.seekg(0);
		ff.read( (char*) &worldCoords, sizeof(worldCoords) );
		ff.read( (char*) &player, sizeof(player) );
		
		// invalidate position snapshot
		player.snapX = player.snapY = player.snapZ = 0;
	}
	void World::save()
	{
		std::ofstream ff (worldFolder() + "/world.data", std::ios::trunc | std::ios::binary);
		if (!ff) return;
		
		ff.seekp(0);
		ff.write( (char*) &worldCoords, sizeof(worldCoords) );
		ff.write( (char*) &player, sizeof(player) );
	}
	
}
