#include "render_gui.hpp"

#include "library/math/matrix.hpp"
#include "library/opengl/opengl.hpp"
#include "library/opengl/window.hpp"
#include "library/opengl/oglfont.hpp"
#include "renderman.hpp"
#include "shaderman.hpp"
#include <cmath>
#include <sstream>

using namespace library;

namespace cppcraft
{
	GUIRenderer rendergui;
	const double PI = 4 * atan(1);
	// the GUIs orthographic projection matrix
	mat4 ortho;
	// A wild Font appears!
	OglFont font;
	
	void GUIRenderer::init(Renderer& renderer)
	{
		width  = 1.0;
		height = 1.0 / renderer.gamescr.SA;
		
		// orthographic projection
		ortho = orthoMatrix(width, height, 0, 2);
		
		initInventoryRenderer();
		
		// initialize our font renderer
		font.load("bitmap/default/gui/font.png", 16);
		
		font.createDefaultShader();
		font.bind(0);
		font.sendMatrix(ortho);
		
		font.setColor(vec4(0.8, 0.8, 1.0, 1.0));
		font.setBackColor(vec4(0.0, 0.5));
	}
	
	inline void guiPrinter(const vec3& position, const vec2& size, std::string text)
	{
		font.bind(0);
		font.print(position, size, text);
	}
	
	void GUIRenderer::render(Renderer& renderer)
	{
		// clear depth buffer
		glDepthMask(GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		/// player hand ///
		renderPlayerhand(renderer.frametick);
		
		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		
		/// compass & minimap ///
		renderMinimap(ortho);
		
		/// crosshair ///
		renderCrosshair(ortho);
		
		/// quickbar ///
		renderQuickbar(ortho, renderer.frametick);
		
		glDisable(GL_BLEND);
		
		/// quickbar items ///
		renderQuickbarItems(ortho, renderer.frametick);
		
		/// test text ///
		glEnable(GL_BLEND);
		
		guiPrinter(vec3(0.01, 0.01, 0.0), vec2(0.01), "cppcraft v0.1");
		
		std::stringstream ss;
		ss << "fps: " << renderer.FPS;
		guiPrinter(vec3(0.01, 0.02, 0.0), vec2(0.01), ss.str());
		
		glDisable(GL_BLEND);
	}
	
	
}
