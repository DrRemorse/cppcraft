#include "player_actions.hpp"

namespace cppcraft
{
	PlayerActions paction;
	// timing intervals
	const double PlayerActions::MINE_SPEED = 0.04;
	
	void PlayerActions::init()
	{
		action = PA_Nothing;
		mineTimer = 0;
		actionTimer  = 0;
		cooldownTime = 0;
	}
	
	void PlayerActions::handle(double frametime)
	{
		this->handleInputs(frametime);
		this->handleActions(frametime);
		
		if (action == playeraction_t::PA_Cooldown)
		{
			if (frametime > actionTimer + MINE_SPEED)
			{
				actionTimer = frametime;
				
				if (mineTimer)
				{
					mineTimer -= 1;
				}
				else
				{
					action = PA_Nothing;
					cancelDig(); // cancel after action != cooldown
					cooldownTime = frametime;
				}
			}
		}
	}
	
	PlayerActions::playeraction_t PlayerActions::getAction() const
	{
		return this->action;
	}
	
	void PlayerActions::cancelDig()
	{
		if (action != playeraction_t::PA_Cooldown)
		{
			action = playeraction_t::PA_Nothing;
			/*
			if (network.mymine == true)
			{
				networkSend("PMINE 0");
				network.mymine = false;
			}*/
		}
	}
	
	void PlayerActions::handWave()
	{
		action = playeraction_t::PA_Cooldown;
		mineTimer = MINE_COOLDOWN;
		/*
		cooldowntime = fru.uptimed
		if (network.mymine = false)
		{
			networkSend("PMINE 1")
			network.mymine = TRUE
		}*/
	}
	
	void PlayerActions::cooldown(double frametime)
	{
		action = PA_Cooldown;
		mineTimer = MINE_COOLDOWN;
		cooldownTime = frametime;
	}
	
	void PlayerActions::swingTool(InventoryItem& item)
	{
		action = playeraction_t::PA_Swingtool;
		actionTimer = 8;
		
	}
	
	
}