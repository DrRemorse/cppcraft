#include "player_actions.hpp"

#include "player_inputs.hpp"
#include <GL/glfw3.h>

using namespace library;

namespace cppcraft
{
	void PlayerActions::handleInputs()
	{
		static bool lock_left_axis = false;
		static bool lock_right_axis = false;
		
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
		else if (keyconf.jaxis[2] > 0.5)
		{
			if (lock_left_axis == false)
			{
				// lock axis
				lock_left_axis = true;
				
				// add block
				handWave();
				action = PA_Addblock;
			}
		}
		// right trigger axis
		else if (keyconf.jaxis[2] < -0.5)
		{
			if (lock_right_axis == false)
			{
				// lock this button
				lock_right_axis = true;
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
			lock_left_axis = false;
			lock_right_axis = false;
			// cancel stuff
			cancelDig();
		}
	}
	
}
