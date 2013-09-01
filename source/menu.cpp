#include "menu.hpp"

#include "blocks.hpp"
#include "items.hpp"

namespace cppcraft
{
	MenuClass menu;
	Inventory inventory;
	
	void MenuClass::init()
	{
		// initialize items
		items.init();
		
		// quickbar: lowest row on inventory
		this->quickbarX = 0;
		this->quickbarY = 4;
		
		inventory = Inventory(9, 5);
		
		// create default inventory
		for (int x = 0; x < inventory.getWidth() / 2; x++)
		{
			inventory(x, quickbarY) = InventoryItem(IT_WOODPICK + x, ITT_ITEM, 64);
		}
		for (int x = inventory.getWidth() / 2; x < inventory.getWidth(); x++)
		{
			inventory(x, quickbarY) = InventoryItem(_WOODBROWN + x - inventory.getWidth() / 2, ITT_BLOCK, 64);
		}
		inventory.setChanged(true);
		
	}
	
	Inventory::Inventory(int w, int h)
	{
		this->width = w;
		this->height = h;
		this->items = new InventoryItem[w * h]();
	}
	
	InventoryItem& Inventory::operator() (int x, int y)
	{
		return items[width * y + x];
	}
	
}