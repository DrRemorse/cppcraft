#include "player.hpp"

#include <library/config.hpp>
#include <library/log.hpp>
#include <library/opengl/input.hpp>
#include "chat.hpp"
#include "menu.hpp"
#include "network.hpp"
#include "player_inputs.hpp"
#include "sectors.hpp"
#include "sun.hpp"
#include "threading.hpp"
#include "worldbuilder.hpp"
#include <GL/glfw3.h>
#include <cmath>

using namespace library;

namespace cppcraft
{
	keyconf_t keyconf;
	Input     input;
	
	void PlayerClass::initInputs(WindowClass& gameScreen)
	{
		logger << Log::INFO << "* Initializing input systems" << Log::ENDL;
		
		keyconf.k_forward  = config.get("k_forward",  87); // W
		keyconf.k_backward = config.get("k_backward", 83); // S
		keyconf.k_right    = config.get("k_right",    68); // D
		keyconf.k_left     = config.get("k_left",     65); // A
		
		keyconf.k_jump   = config.get("k_jump", 32);    // Space
		keyconf.k_sprint = config.get("k_sprint", GLFW_KEY_LEFT_SHIFT);
		keyconf.k_crouch = config.get("k_crouch", GLFW_KEY_LEFT_CONTROL);
		keyconf.k_throw  = config.get("k_throw", 81);   // Q
		
		keyconf.k_flying  = config.get("k_flying", 70);  // F
		keyconf.k_flyup   = config.get("k_flyup",   84); // T
		keyconf.k_flydown = config.get("k_flydown", 82); // R
		
		keyconf.k_inventory = config.get("k_inventory", 73); // I
		
		double mspd  = config.get("m_speed", 120) / 1000.0;
		double msens = config.get("m_sens",  80)  / 10.0;
		
		// initialize input systems
		input.init(gameScreen, true, true);
		input.setRotation(vec2(player.xrotrad, player.yrotrad));
		input.grabMouse(true);  // enable fps-like mouse
		input.mouseOptions(mspd, msens); // mouse speed & sensitivity
		input.showMouse(false); // hide mouse
	}
	
	void PlayerClass::handleInputs()
	{
		if (busyControls() == false)
		{
			if (input.getKey(GLFW_KEY_F1))
			{
				thesun.setRadianAngle(0.1);
				Sectors.invalidateAll();
				worldbuilder.reset();
			}
			if (input.getKey(GLFW_KEY_F2))
			{
				thesun.setRadianAngle(0.707);
				Sectors.invalidateAll();
				worldbuilder.reset();
			}
			if (input.getKey(GLFW_KEY_F3))
			{
				thesun.setRadianAngle(3.0);
				Sectors.invalidateAll();
				worldbuilder.reset();
			}
			if (input.getKey(GLFW_KEY_F4))
			{
				thesun.setRadianAngle(-1);
				Sectors.invalidateAll();
				worldbuilder.reset();
			}
			
			if (input.getKey(GLFW_KEY_C))
			{
				Sector* sector = Sectors.sectorAt(player.X, player.Y, player.Z);
				
				if (sector)
				{
					logger << Log::INFO << "Sector (" << sector->x << ", " << sector->y << ", " << sector->z << "): " << (int) sector->progress << Log::ENDL;
					logger << Log::INFO << "culled= " << (int) sector->culled << ", render= " << sector->render << ", content= " << (int) sector->contents << Log::ENDL;
				}
			}
			
			if (input.getKey(keyconf.k_flying))
			{
				if (input.getKey(keyconf.k_flying) != Input::KEY_LOCKED)
				{
					// lock key
					input.hold(keyconf.k_flying);
					// toggle flying
					player.Flying = ! player.Flying;
				}
			}
			
			int wheel = input.getWheel();
			if (wheel > 0)
			{
				// previous quickbar item
				menu.quickbarX = (menu.quickbarX + 1) % inventory.getWidth();
			}
			else if (wheel < 0)
			{
				// go to next inventory item in quickbar
				if (menu.quickbarX)
					menu.quickbarX -= 1;
				else
					menu.quickbarX = inventory.getWidth()-1;
			}
			// number keys (1-9) to directly select on quickbar
			for (int i = 1; i < 10; i++)
			if (input.getKey(GLFW_KEY_0 + i))
			{
				menu.quickbarX = (i - 1) % inventory.getWidth();
			}
			
		} // busyControls
		
		if (input.getKey(GLFW_KEY_ESCAPE) == Input::KEY_PRESSED)
		{
			input.hold(GLFW_KEY_ESCAPE);
			
			if (chatbox.isOpen())
			{
				chatbox.openChat(false);
			}
			else
			{
				mtx.terminate = true;
			}
		}
		
		if (input.getKey(GLFW_KEY_ENTER) == Input::KEY_PRESSED)
		{
			input.hold(GLFW_KEY_ENTER);
			chatbox.openChat(!chatbox.isOpen());
			
			if (chatbox.isOpen() == false)
			{
				// say something, as long as its something :)
				if (input.getText().size())
					network.sendChat(input.getText());
			}
			input.clearText();
		}
		
		if (input.getKey(GLFW_KEY_BACKSPACE) == Input::KEY_PRESSED)
		{
			input.hold(GLFW_KEY_BACKSPACE);
			if (chatbox.isOpen())
				input.textBackspace();
		}
		
	}
}
