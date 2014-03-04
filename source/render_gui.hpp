#ifndef RENDER_GUI_HPP
#define RENDER_GUI_HPP

namespace library
{
	class mat4;
	class SimpleFont;
}

namespace cppcraft
{
	class Renderer;
	
	class GUIRenderer
	{
	public:
		struct gui_vertex_t
		{
			float x, y, z;
			float u, v;
			unsigned int color;
		};
		
		void init(Renderer& renderer);
		void render(Renderer& renderer);
		
		library::SimpleFont& getFont();
		
	private:
		float width;
		float height;
		
		void initInventoryRenderer();
		
		void renderPlayerhand(double frameCounter);
		void renderMinimap(library::mat4& ortho);
		void renderCrosshair(library::mat4& ortho);
		void renderQuickbar(library::mat4& ortho, double frameCounter);
		void renderQuickbarItems(library::mat4& ortho, double frameCounter);
	};
	extern GUIRenderer rendergui;
}
#endif
