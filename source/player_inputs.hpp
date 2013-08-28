#ifndef PLAYER_INPUTS_HPP
#define PLAYER_INPUTS_HPP

namespace cppcraft
{
	struct keyconf_t
	{
		int k_forward;
		int k_backward;
		int k_right;
		int k_left;
		
		int k_jump;
		int k_crouch;
		int k_sprint;
		int k_throw;
		int k_inventory;
		
		int k_flying;
		int k_flyup;
		int k_flydown;
		
		bool alternateMiningButton;
	};
	extern keyconf_t keyconf;
}

#endif
