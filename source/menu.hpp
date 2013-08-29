#ifndef MENU_HPP
#define MENU_HPP

#include "items.hpp"

namespace cppcraft
{
	class MenuClass
	{
	public:
		static const int INV_WIDTH  = 9;
		static const int INV_HEIGHT = 5;
		
		enum menumode_t
		{
			NO_MENU,
			MENU_INVENTORY,
			MENU_WORKBENCH,
			MENU_CHEST,
			MENU_FURNACE,
			
			MENU_CRAFTBOX,
			MENU_CRAFTRES,
			
			MENU_GAMEMENU
			
		};
		
		menumode_t menuMode;
		
		int inventoryAction;
		int quickbarX = 0;            // (x) quickbar current column
		int quickbarY = INV_HEIGHT-1; // (y) quickbar current row
		
		InventoryItem inventoryItems[INV_WIDTH * INV_HEIGHT];
		
		int selectionX, selectionY;
		
		void init();
		InventoryItem& inventory(int x, int y);
		
	};
	extern MenuClass menu;
	
}

#endif
