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
		inventory(4, quickbarY) = InventoryItem(_WOODDOOR, ITT_BLOCK, 64);
		inventory(5, quickbarY) = InventoryItem(_PLANK, ITT_BLOCK, 64);
		inventory(6, quickbarY) = InventoryItem(_LADDER, ITT_BLOCK, 64);
		inventory(7, quickbarY) = InventoryItem(_TORCH, ITT_BLOCK, 64);
		inventory(8, quickbarY) = InventoryItem(_LANTERN, ITT_BLOCK, 64);
		
		inventory.setChanged(true);
		
	}
	
	InventoryItem& MenuClass::getHeldItem()
	{
		return inventory(quickbarX, quickbarY);
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