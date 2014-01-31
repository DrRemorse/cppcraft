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
	public:
		typedef int wcoord_t;
		
		static const wcoord_t WORLD_SIZE     = 268435456;
		static const wcoord_t WORLD_NODESIZE = 256;
		static const wcoord_t WORLD_CENTER   = WORLD_SIZE / 2;
		
		static const wcoord_t WORLD_STARTING_X = WORLD_CENTER + WORLD_NODESIZE / 2;
		static const wcoord_t WORLD_STARTING_Z = WORLD_STARTING_X;
		
		struct world_t
		{
		public:
			world_t() {}
			world_t(wcoord_t wx, wcoord_t wy, wcoord_t wz)
				: worldX(wx), worldY(wy), worldZ(wz) {}
			
			wcoord_t worldX, worldY, worldZ;
		};
		
		// initializes the world
		void init(std::string& folder);
		// returns current world folder
		inline const std::string& worldFolder() const
		{
			return this->folder;
		}

		inline wcoord_t getWX() const { return worldCoords.worldX; }
		inline wcoord_t getWY() const { return worldCoords.worldY; }
		inline wcoord_t getWZ() const { return worldCoords.worldZ; }
		
		void load();
		void save();
		
		// seamless transitions the word, and is the only one
		// allowed to directly modify worldX, worldZ
		friend Seamless; // special customer
		
	private:
		std::string folder;
		world_t worldCoords;
	};
	extern World world;
}

#endif
