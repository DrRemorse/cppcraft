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
		
		inventory.create(9, 5);
		
		// create default inventory
		inventory(0, quickbarY) = InventoryItem(IT_DIAMPICK, ITT_ITEM, 1);
		
		inventory(1, quickbarY) = InventoryItem(_STONE, ITT_BLOCK, 9999);
		inventory(2, quickbarY) = InventoryItem(_PLANK, ITT_BLOCK, 9999);
		
		inventory(3, quickbarY) = InventoryItem(_WOODSTAIR, ITT_BLOCK, 999);
		inventory(4, quickbarY) = InventoryItem(_WOODPOLE, ITT_BLOCK, 999);
		
		inventory(5, quickbarY) = InventoryItem(_WOODDOOR, ITT_BLOCK, 8);
		inventory(6, quickbarY) = InventoryItem(_LADDER, ITT_BLOCK, 999);
		
		inventory(7, quickbarY) = InventoryItem(_TORCH, ITT_BLOCK, 255);
		inventory(8, quickbarY) = InventoryItem(_LANTERN, ITT_BLOCK, 255);
		
		inventory.setChanged(true);
		
	}
	
	InventoryItem& MenuClass::getHeldItem()
	{
		return inventory(quickbarX, quickbarY);
	}
	
	Inventory::Inventory(int w, int h)
		: Inventory()
	{
		create(w, h);
	}
	void Inventory::create(int w, int h)
	{
		this->width = w;
		this->height = h;
		this->items = new InventoryItem[w * h]();
	}
	Inventory::~Inventory()
	{
		delete[] this->items;
	}
	
	InventoryItem& Inventory::operator() (int x, int y)
	{
		return items[width * y + x];
	}
	
}