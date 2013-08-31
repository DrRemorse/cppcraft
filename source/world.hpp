/**
 * WorldClass
 * 
 * Contains information about world source and the location in 
 * this huge-finite world. 'folder' is the directory which the worlds
 * world-data is contained in, including some "permanent" player information.
 * 
 * The WORLD coordinates are measured in sectors (of Sector::BLOCKS_XZ blocks)
 * and represents the (0, 0) local sector corner for player view, or alternatively
 * the (0, 0, 0) (left, bottom, back) corner in a 3D setting.
 * 
 * Example transition +x:
 * The player is moved backward exactly ONE sector, and worldX increased
 * by +1, when player reaches the seamless OFFSET treshold.
 * 
**/

#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>

namespace cppcraft
{
	class Seamless; // proto forward
	
	class World
	{
		std::string folder;
		int worldX;
		int worldY;
		int worldZ;
		
	public:
		static const int WORLD_SIZE     = 268435456;
		static const int WORLD_NODESIZE = 256;
		static const int WORLD_CENTER   = WORLD_SIZE / 2;
		
		static const int WORLD_STARTING_X = WORLD_CENTER + WORLD_NODESIZE / 2;
		static const int WORLD_STARTING_Z = WORLD_STARTING_X;
		
		// initializes the world
		void init(std::string& folder);
		// returns current world folder
		const std::string& worldFolder() const;
		
		int getWX() { return this->worldX; }
		int getWY() { return this->worldY; }
		int getWZ() { return this->worldZ; }
		
		void load();
		void save();
		
		// seamless transitions the word, and is the only one
		// allowed to directly modify worldX, worldZ
		friend Seamless; // special customer
	};
	extern World world;
}

#endif
