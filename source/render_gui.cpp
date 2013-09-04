#include "render_gui.hpp"

#include "library/math/matrix.hpp"
#include "library/opengl/opengl.hpp"
#include "library/opengl/window.hpp"
#include "renderman.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	GUIRenderer rendergui;
	const double PI = 4 * atan(1);
	// the GUIs orthographic projection matrix
	Matrix ortho;
	
	void GUIRenderer::init(Renderer& renderer)
	{
		width  = 1.0;
		height = 1.0 / renderer.gamescr.SA;
		
		// orthographic projection
		ortho.ortho(width, height, 0, 2);
		
		initInventoryRenderer();
	}
	
	void GUIRenderer::render(Renderer& renderer)
	{
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		/// player hand ///
		renderPlayerhand(renderer.frametick);
		
		glEnable(GL_BLEND);
		
		/// compass & minimap ///
		renderMinimap(ortho);
		
		/// crosshair ///
		renderCrosshair(ortho);
		
		/// quickbar ///
		renderQuickbar(ortho, renderer.frametick);
		
		glDisable(GL_BLEND);
		
		/// quickbar items ///
		renderQuickbarItems(ortho, renderer.frametick);
		
	}
	
	
}
