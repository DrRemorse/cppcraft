#ifndef PLAYER_HPP
#define PLAYER_HPP

#define DBG_FLYING

#include "library/math/vector.hpp"
#include "blocks.hpp"

namespace cppcraft
{
	class Sector;
	class InventoryItem;
	
	class PlayerClass
	{
	public:
		// position
		double X, Y, Z;
		double snapX, snapY, snapZ;
		int    snapStage;
		
		// rotation
		float xrot, yrot;
		float xrotrad, yrotrad;
		
		// acceleration
		double pax, pay, paz;
		// no-gravity toggle flag
		bool Flying;
		// true if player just moved, update camera etc.
		bool JustMoved;
		
		void initPlayer();
		void initInputs();
		// first we run this, to get inputs
		void handleInputs();
		// then, for each tick passed we run this
		void handlePlayerTicks();
		// then finally, we finish by running this once
		void handleActions(double frametime);
		
		// signals rotation of player, used for networking
		void rotated();
		// returns the player look vector
		library::vec3 getLookVector() const;
		// returns the cube face that would face the player
		block_t getBlockFacing() const;
		// returns the terrain id for current player position (x, z)
		int getTerrain() const;
	};
	extern PlayerClass player;
	
}

#endif
