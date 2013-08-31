#ifndef RENDER_GUI_INVENTORY
#define RENDER_GUI_INVENTORY

#include "library/opengl/vao.hpp"
#include "menu.hpp"
#include "render_gui.hpp"
#include <vector>

namespace library
{
	class Matrix;
}

namespace cppcraft
{
	class GUIInventory
	{
	public:
		struct inventory_t
		{
			float x, y, z;
			float u, v, w;
			unsigned int color;
		};
	
		void clear();
		int  emit(InventoryItem& itm, float x, float y, float size);
		void render(library::Matrix& ortho);
		
	private:
		library::VAO itemsVAO;
		library::VAO blocksVAO;
		
		std::vector<inventory_t> blockTiles;
		std::vector<inventory_t> itemTiles;
		
		int emitQuad(InventoryItem& itm, float x, float y, float size, float tile);
		int emitBlock(InventoryItem& itm, float x, float y, float size);
	};
}

#endif
