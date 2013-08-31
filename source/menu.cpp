#include "menu.hpp"

#include "blocks.hpp"
#include "items.hpp"

namespace cppcraft
{
	MenuClass menu;
	
	void MenuClass::init()
	{
		// initialize items
		items.init();
		
		// create default inventory
		for (int x = 0; x < INV_WIDTH; x++)
		{
			inventory(x, quickbarY) = InventoryItem(IT_WOODPICK + x, ITT_ITEM, 64);
			//inventory(x, quickbarY) = InventoryItem(IT_WOODPICK + x, ITT_ITEM, 64);
		}
		
	}
	
	InventoryItem& MenuClass::inventory(int x, int y)
	{
		return this->inventoryItems[INV_WIDTH * y + x];
	}
	
}