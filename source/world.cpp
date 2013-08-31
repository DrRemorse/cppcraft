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
		// first, initialize sectors & blocks
		int sectors_xz = config.get("sectors_axis", 48);
		Sectors.createSectors(sectors_xz);
		// also, initialize 2D flatlands (fsectors)
		Flatlands.init();
		// then, initialize world coordinates centering player in the world
		this->worldX = WORLD_STARTING_X - sectors_xz / 2; //+ 84; //
		this->worldY = 0;
		this->worldZ = WORLD_STARTING_Z - sectors_xz / 2; //- 104; //
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
	
	const std::string& World::worldFolder() const
	{
		return this->folder;
	}
	
	struct savetheworld_t
	{
		int worldX, worldY, worldZ;
	};
	
	void World::load()
	{
		std::ifstream ff (world.worldFolder() + "/world.data", std::ios::in | std::ios::binary);
		if (!ff) return;
		
		savetheworld_t w;
		
		ff.seekg(0);
		ff.read( (char*) &w, sizeof(savetheworld_t) );
		ff.read( (char*) &player, sizeof(player) );
		
		this->worldX = w.worldX;
		this->worldY = w.worldY;
		this->worldZ = w.worldZ;
	}
	void World::save()
	{
		std::ofstream ff (world.worldFolder() + "/world.data", std::ios::trunc | std::ios::binary);
		if (!ff) return;
		
		savetheworld_t w = 
		{
			this->worldX, this->worldY, this->worldZ
		};
		
		ff.seekp(0);
		ff.write( (char*) &w, sizeof(savetheworld_t) );
		ff.write( (char*) &player, sizeof(player) );
	}
	
}