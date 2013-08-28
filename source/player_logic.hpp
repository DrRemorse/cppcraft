#ifndef PLAYER_LOGIC_HPP
#define PLAYER_LOGIC_HPP

#include "library/math/vector.hpp"

namespace cppcraft
{
	class Block;
	class Sector;
	class InventoryItem;
	
	enum playeraction_t
	{
		PA_Nothing   = 0,
		PA_Addblock  = 1,
		PA_Mineblock = 2,
		PA_Remblock  = 3,
		PA_Cooldown  = 4,
		PA_Use       = 5,
		PA_Swingtool = 6
		
	};
	
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
		// focal position
		// float focal;
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
		bool Rotate;
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
		
		// mining related
		float cooldownTime;
		unsigned short mineMax;
		unsigned short mineTimer;
		double mineTiming;
		int    minimizer;
		// action related
		playeraction_t action;
		float actionTimer;
		
		static const int MINE_SOUNDMOD = 6;
		static const int MINE_COOLDOWN = 4;
		static const double MINE_SPEED;
		int lastlanding;
		
		// player standing on this:
		Block* block;
		Block* lastblock;
		
		// timers
		int lastwater;
		
		// hand interpolation
		library::vec3 lasthand; // vec3?
		
		// declarations
		void cancelDig(); // cancel digging (networked)
		void handWave();  // wave hand (networked)
		
		// returns true if the player has selected a block in the world
		bool hasSelection() const;
		
		
		/// all movement & speed related ///
		void translatePlayer();
		void handlePlayerJumping();
		/// all action related ///
		void handleActions();
		// activate a block in the world
		void activate(InventoryItem& item);
		// perform an action with an item
		void itemAction(InventoryItem& item);
		// place a block
		void build(InventoryItem& item);
		void swingTool(InventoryItem& item);
		// placing blocks shite
		bool playerBlockPlaceTest();
		
		// walking / etc. sounds
		void playerSounds();
		
		unsigned short determineSelectionFacing(Block& block, library::vec3& ray, library::vec3& fracs, float stepSize);
	};
	extern PlayerLogic plogic;
}

#endif
