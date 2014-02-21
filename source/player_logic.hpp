#ifndef PLAYER_LOGIC_HPP
#define PLAYER_LOGIC_HPP

#include "library/math/vector.hpp"

namespace cppcraft
{
	class Block;
	class Sector;
	
	enum movestate_t
	{
		PMS_Normal,
		PMS_Crouch,
		PMS_Sprint
		
	};
	
	struct playerselect_t
	{
		// selected sector
		Sector* sector;
		// selected block
		Block*  block;
		// grid position
		library::vec3 pos;
		// selection direction
		unsigned short facing;
		// additional info
		int info[2];
		// inform rendering thread on updates
		int checkSum;
		bool updated;
	};
	
	class PlayerLogic
	{
	public:
		enum playerSubmerged_t
		{
			PS_None,
			PS_Water,
			PS_Lava
		};
		
		// players current sector
		Sector* sector;
		movestate_t movestate;
		
		// players current shadow/color
		unsigned int shadowColor;
		unsigned int torchColor;
		// current terrain type
		int terrain;
		
		// selection
		playerselect_t selection;
		
		// temporary for fall testing
		bool freefall;
		// water escape mechanism
		bool EscapeAttempt;
		// status flags
		bool Falling;
		bool Ladderized;
		bool Slowfall;
		bool Moved;
		int  Motion;
		playerSubmerged_t Submerged;
		playerSubmerged_t FullySubmerged;
		
	#ifdef USE_JETPACK
		// jetpacking
		bool canJetpack;
		bool jetpacking;
		
		short jetpack;
		short lastjet;
		static const short MAX_JETPACK = 32;
		static const short JETPACK_SOUNDWAIT = 24;
	#endif
		
		// player standing on this:
		Block* block;
		Block* lastblock;
		
		// returns true if the player has selected a block in the world
		bool hasSelection() const;
		
		/// all movement & speed related ///
		void translatePlayer();
		void handlePlayerJumping();
		
		// walking / etc. sounds
		void playerSounds();
		
		unsigned short determineSelectionFacing(Block& block, library::vec3& ray, library::vec3& fracs, float stepSize);
	};
	extern PlayerLogic plogic;
}

#endif
