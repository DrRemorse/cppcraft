#include "player_logic.hpp"

#include "library/math/toolbox.hpp"
#include "player.hpp"
#include "soundman.hpp"

using namespace library;

namespace cppcraft
{
	// PlayerLogicClass
	PlayerLogic plogic;
	// sound timing intervals
	///...
	
	bool PlayerLogic::hasSelection() const
	{
		return (selection.block != nullptr);
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
