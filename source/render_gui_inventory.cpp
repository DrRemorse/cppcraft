#include "render_gui_inventory.hpp"

#include "library/log.hpp"
#include "library/bitmap/colortools.hpp"
#include "library/math/matrix.hpp"
#include "library/opengl/opengl.hpp"
#include "blocks.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"

using namespace library;

namespace cppcraft
{
	GUIInventory quickbarItems;
	
	void GUIRenderer::renderQuickbarItems(library::Matrix& ortho, double frameCounter)
	{
		float width = this->width * 0.4;
		float height = width / 8;
		float posx = (this->width - width) * 0.5;
		float posy = this->height - height;
		
		posx += width * 0.025;
		posy += width * 0.025;
		
		float size   = width * 0.075;
		float stride = width * 0.1095;
		
		if (inventory.isChanged())
		{
			// inventory has no longer changed
			inventory.setChanged(false);
			
			// recreate meshes
			quickbarItems.clear();
			
			for (int x = 0; x < inventory.getWidth(); x++)
			{
				InventoryItem& itm = inventory(x, menu.quickbarY);
				quickbarItems.emit(itm, posx + x * stride, posy, size);
			}
			quickbarItems.upload();
		}
		
		// render inventory
		quickbarItems.render(ortho);
	}
	
	void GUIInventory::clear()
	{
		this->blockTiles.clear();
		this->itemTiles.clear();
	}
	
	int GUIInventory::emit(InventoryItem& itm, float x, float y, float size)
	{
		if (itm.getType() == ITT_ITEM)
		{
			// texture tile id
			float tile = items.tileByID(itm.getID());
			
			return quickbarItems.emitQuad(itm, x, y, size, tile);
		}
		else if (itm.getType() == ITT_BLOCK)
		{
			return quickbarItems.emitBlock(itm, x, y, size);
		}
		return 0;
	}
	
	// FIXME: SLOW AS FUCK
	int GUIInventory::emitQuad(InventoryItem& itm, float x, float y, float size, float tile)
	{
		// no item, no texture!
		if (itm.getID() == IT_NONE) return 0;
		// no count, no item!
		if (itm.getCount() == 0) return 0;
		
		// create single quad
		itemTiles.emplace_back( (inventory_t)
			{ x,        y + size, 0,   0, 0, tile,   BGRA8(255, 255, 255,   0) });
		itemTiles.emplace_back( (inventory_t)
			{ x + size, y + size, 0,   1, 0, tile,   BGRA8(255, 255, 255,   0) });
		itemTiles.emplace_back( (inventory_t)
			{ x + size, y,        0,   1, 1, tile,   BGRA8(255, 255, 255,   0) });
		itemTiles.emplace_back( (inventory_t)
			{ x,        y,        0,   0, 1, tile,   BGRA8(255, 255, 255, 128) });
		
		return 4;
	}
	
	int GUIInventory::emitBlock(InventoryItem& itm, float x, float y, float size)
	{
		// texture tile id
		float tile = Block::cubeFaceById(itm.getID(), 0, 0);
		
		return 0;
	}
	
	void GUIInventory::upload()
	{
		int items  = itemTiles.size();
		int blocks = blockTiles.size();
		
		if (items + blocks == 0) return;
		
		// add blocks to the end of items
		if (blocks)
		{
			itemTiles.insert(itemTiles.end(), blockTiles.begin(), blockTiles.end());
		}
		
		/// upload blocks & items ///
		if (itemsVAO.isGood() == false)
		{
			itemsVAO.begin(sizeof(inventory_t), itemTiles.size(), itemTiles.data());
			itemsVAO.attrib(0, 3, GL_FLOAT, GL_FALSE, offsetof(inventory_t, x));
			itemsVAO.attrib(1, 3, GL_FLOAT, GL_FALSE, offsetof(inventory_t, u));
			itemsVAO.attrib(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(inventory_t, color));
			itemsVAO.end();
		}
		else
		{
			itemsVAO.begin(sizeof(inventory_t), itemTiles.size(), itemTiles.data());
			itemsVAO.end();
		}
	}
	
	void GUIInventory::render(Matrix& ortho)
	{
		int total  = itemTiles.size();
		
		// nothing to do here with no items or blocks
		if (total == 0) return;
		
		int blocks = blockTiles.size();
		int items  = total - blocks;
		
		/// render all menu items ///
		shaderman[Shaderman::MENUITEM].bind();
		shaderman[Shaderman::MENUITEM].sendMatrix("mvp", ortho);
		
		if (items)
		{
			// items texture
			textureman.bind(0, Textureman::T_ITEMS);
			// render items
			itemsVAO.render(GL_QUADS, 0, items);
		}
		if (blocks)
		{
			// blocks texture
			textureman.bind(0, Textureman::T_DIFFUSE);
			// render blocks
			itemsVAO.render(GL_QUADS, items, blocks);
		}
	}
	
}