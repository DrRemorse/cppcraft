#ifndef MENU_HPP
#define MENU_HPP

namespace cppcraft
{
	class MenusClass
	{
		const int INV_WIDTH  = 9;
		const int INV_HEIGHT = 5;
		
		typedef enum
		{
			NO_MENU,
			MENU_INVENTORY,
			MENU_WORKBENCH,
			MENU_CHEST,
			MENU_FURNACE,
			
			MENU_CRAFTBOX,
			MENU_CRAFTRES,
			
			MENU_GAMEMENU
			
		} menumode_t;
		
		int inventoryAction;
		int invcycle = 0;            // (x) quickbar selected item
		int quickbar = INV_HEIGHT-1; // (y) quickbar current row
		
		menumode_t menuMode;
		
		int selection;
		int selectionX, selectionY;
		
	};
	extern as menu_globals menu
	
}

#endif
