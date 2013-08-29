#include "player_actions.hpp"

#include "input.hpp"
#include <GL/glfw3.h>

namespace cppcraft
{
	void PlayerActions::handleInputs(double frametime)
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
		else
		{
			// cancel stuff
			cancelDig();
		}
	}
	
}