#ifndef ITEMS_HPP
#define ITEMS_HPP

/**
 * 
 * 
 * 
**/

#include <string>

namespace cppcraft
{
	const int MAX_UNIQUE_ITEMS = 256;
	
	#define IT_NONE         0
	
	#define IT_WOODPICK     1
	#define IT_STONEPICK    2
	#define IT_IRONPICK     3
	#define IT_GOLDPICK     4
	#define IT_DIAMPICK     5
	
	#define IT_WOODSWORD   10
	#define IT_STONESWORD  11
	#define IT_IRONSWORD   12
	#define IT_GOLDSWORD   13
	#define IT_DIAMSWORD   14
	
	#define IT_WOODSHOVEL  20
	#define IT_STONESHOVEL 21
	#define IT_IRONSHOVEL  22
	#define IT_GOLDSHOVEL  23
	#define IT_DIAMSHOVEL  24
	
	#define IT_WOODAXE     30
	#define IT_STONEAXE    31
	#define IT_IRONAXE     32
	#define IT_GOLDAXE     33
	#define IT_DIAMAXE     34
	
	#define IT_WOODSCYTHE  40
	
	#define IT_COAL        50
	#define IT_IRON        51
	#define IT_GOLD        52
	#define IT_DIAMOND     53
	#define IT_REDSTONE    54
	#define IT_MAGIC       55
	
	#define IT_STICK      100
	
	#define IT_PORTALGUN  190
	
	#define IT_TORCH      200
	
	#define IT_CREATORGUN 255
	
	typedef unsigned short item_t;
	
	typedef enum
	{
		ITT_BLOCK,
		ITT_ITEM,
		ITT_UNKNOWN
		
	} itemtype_t;
	
	class InventoryItem
	{
	public:
		static const int MAX_STACK = 64;
		
		item_t id;
		unsigned short count;
		unsigned char special;
		unsigned char type;
		unsigned char health;
		
		InventoryItem();
		InventoryItem(item_t itemID, unsigned short icount);
		InventoryItem(item_t itemID, itemtype_t itype, unsigned short icount);
		
		bool isActionItem() const;
	};
	
	class ItemsClass
	{
	public:
		int itemW, itemH;
		int itemsX, itemsY;
		
		int tileByID(int id);
		
		std::string getName(item_t id);
	};
	extern ItemsClass items;
}

#endif
