#include "menu.hpp"

#include "blocks.hpp"

namespace cppcraft
{
	void MenuClass::init()
	{
		// create default inventory
		for (int x = 0; x < INV_WIDTH; x++)
		{
			inventory(x, quickbarY) = InventoryItem(_COAL + x, 64);
		}
		
	}
	
	InventoryItem& MenuClass::inventory(int x, int y)
	{
		return this->inventoryItems[INV_WIDTH * y + x];
	}
	
}