#include "player_actions.hpp"

#include "player_inputs.hpp"
#include <GL/glfw3.h>

using namespace library;

namespace cppcraft
{
	void PlayerActions::handleInputs()
	{
		static bool lock_hand_axes = false;
		
		// left mouse button
		if (input.getMouse(GLFW_MOUSE_BUTTON_1))
		{
			if (input.getMouse(GLFW_MOUSE_BUTTON_1) == Input::KEY_PRESSED)
			{
				// lock this button
				input.holdMouse(GLFW_MOUSE_BUTTON_1);
				
				// add block
				handWave();
				action = PA_Addblock;
			}
		}
		else if (input.getMouse(GLFW_MOUSE_BUTTON_2))
		{
			if (input.getMouse(GLFW_MOUSE_BUTTON_2) == Input::KEY_PRESSED)
			{
				// lock this button
				input.holdMouse(GLFW_MOUSE_BUTTON_2);
				// enable mining
				handWave();
				mineTimer = 2000;
				mineMax   = 2000;
				minimizer = -1; // CRC
				action = PA_Mineblock;
			}
		}
		// left trigger axis
		else if (keyconf.jaxis[keyconf.joy_axis_place] > 0.5)
		{
			if (lock_hand_axes == false)
			{
				// lock axis
				lock_hand_axes = true;
				
				// add block
				handWave();
				action = PA_Addblock;
			}
		}
		// right trigger axis
		else if (keyconf.jaxis[keyconf.joy_axis_mine] < -0.5)
		{
			if (lock_hand_axes == false)
			{
				// lock this button
				lock_hand_axes = true;
				// enable mining
				handWave();
				mineTimer = 2000;
				mineMax   = 2000;
				minimizer = -1; // CRC
				action = PA_Mineblock;
			}
		}
		else
		{
			lock_hand_axes = false;
			// cancel stuff
			cancelDig();
		}
	}
	
}
