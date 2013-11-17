#include "player_actions.hpp"

#include "library/opengl/input.hpp"
#include <GL/glfw3.h>

using namespace library;

namespace cppcraft
{
	void PlayerActions::handleInputs()
	{
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
		else
		{
			// cancel stuff
			cancelDig();
		}
	}
	
}
