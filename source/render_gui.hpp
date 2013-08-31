#ifndef RENDER_GUI_HPP
#define RENDER_GUI_HPP

namespace library
{
	class Matrix;
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
		
		void init();
		void render(Renderer& renderer);
		
	private:
		float width;
		float height;
		
		void renderPlayerhand(double frameCounter);
		void renderMinimap(library::Matrix& ortho);
		void renderCrosshair(library::Matrix& ortho);
		void renderQuickbar(library::Matrix& ortho, double frameCounter);
		void renderQuickbarItems(library::Matrix& ortho, double frameCounter);
	};
	extern GUIRenderer rendergui;
}
#endif
