#ifndef PLAYER_ACTIONS_HPP
#define PLAYER_ACTIONS_HPP

namespace cppcraft
{
	class InventoryItem;
	
	class PlayerActions
	{
	public:
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
		
		static const int MINE_SOUNDMOD = 6;
		static const int MINE_COOLDOWN = 4;
		static const double MINE_SPEED;
		
		// initializes this system
		void init();
		// executes all top-level functions for this system
		void handle(double frametime);
		
		void cancelDig(); // cancel digging (networked)
		void handWave();  // wave hand (networked)
		// start cooldown animation
		void cooldown();
		
		/// input related ///
		void handleInputs();
		
		/// all action related ///
		void handleActions(double frametime);
		void handleMining(double frametime, const InventoryItem& helditem);
		// activate a block in the world
		void activate(InventoryItem& item);
		// perform an action with an item
		void itemAction(InventoryItem& item);
		// place a block
		void build(InventoryItem& item);
		void swingTool(const InventoryItem& item);
		// placing blocks shite
		bool playerBlockPlaceTest();
		
		playeraction_t getAction() const;
		
	private:
		// mining related
		double cooldownTime;
		double actionTimer;
		unsigned short mineMax;
		unsigned short mineTimer;
		int minimizer;
		// action related
		playeraction_t action;
		
		
		friend class PlayerHand;
	};
	extern PlayerActions paction;
}

#endif
