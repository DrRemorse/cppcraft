#include "player.hpp"

#include "library/config.hpp"
#include "library/log.hpp"
#include "library/opengl/input.hpp"
#include "menu.hpp"
#include "player_inputs.hpp"
#include "sectors.hpp"
#include "sun.hpp"
#include "threading.hpp"
#include "worldbuilder.hpp"
#include "GL/glfw3.h"
#include <cmath>

using namespace library;

namespace cppcraft
{
	keyconf_t keyconf;
	
	void PlayerClass::initInputs()
	{
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
	}
	
	void PlayerClass::handleInputs()
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
		if (wheel < 0)
		{
			if (menu.quickbarX)
				menu.quickbarX -= 1;
			else
				menu.quickbarX = inventory.getWidth()-1;
		}
		else if (wheel > 0)
		{
			menu.quickbarX = (menu.quickbarX + 1) % inventory.getWidth();
		}
		
		if (input.getKey(GLFW_KEY_ESCAPE))
		{
			mtx.terminate = true;
		}
	}
}
