#include "player_logic.hpp"

#include "library/math/toolbox.hpp"
#include "player.hpp"
#include "soundman.hpp"

using namespace library;

namespace cppcraft
{
	// PlayerLogicClass
	PlayerLogic plogic;
	// timing intervals
	const double PlayerLogic::MINE_SPEED	= 0.04;
	// sound timing intervals
	///...
	
	bool PlayerLogic::hasSelection() const
	{
		return (selection.block != nullptr);
	}
	
	void PlayerLogic::cancelDig()
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
	
	void PlayerLogic::handWave()
	{
		action = playeraction_t::PA_Cooldown;
		mineTimer = plogic.MINE_COOLDOWN;
		/*
		cooldowntime = fru.uptimed
		if (network.mymine = false)
		{
			networkSend("PMINE 1")
			network.mymine = TRUE
		}*/
	}
	
	const int TIME_BETWEEN_CROUCH  = 50;
	const int TIME_BETWEEN_STEPS   = 32;
	const int TIME_BETWEEN_SPRINTS = 20;
	int timesteps = 0;
	
	void PlayerLogic::playerSounds()
	{
		if (Moved) // the player moved
		{
			timesteps++;
			if (movestate == PMS_Crouch)
			{
				if (timesteps > TIME_BETWEEN_CROUCH) timesteps = 0;
			}
			else if (movestate == PMS_Normal)
			{
				if (timesteps > TIME_BETWEEN_STEPS) timesteps = 0;
			}
			else if (movestate == PMS_Sprint)
			{
				if (timesteps > TIME_BETWEEN_SPRINTS) timesteps = 0;
			}
			
			if (timesteps == 0)
			{
				// play material sound
				if (Block::fluidAndCrossToAir(block->getID()) != _AIR)
				{
					int stepmod = toolbox::rnd(4);
					soundman.playMaterial(block->getID(), stepmod);
				}
			}
		}
		else timesteps = TIME_BETWEEN_STEPS / 2;
	}
	
}
