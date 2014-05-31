#include "render_gui.hpp"

#include <library/math/matrix.hpp>
#include <library/opengl/opengl.hpp>
#include <library/opengl/window.hpp>
#include <library/opengl/oglfont.hpp>
#include "chat.hpp"
#include "minimap.hpp"
#include "renderman.hpp"
#include "render_scene.hpp"
#include "shaderman.hpp"
#include <cmath>
#include <sstream>

using namespace library;

namespace cppcraft
{
	GUIRenderer rendergui;
	// the GUIs orthographic projection matrix
	mat4 ortho;
	// A wild Font appears!
	SimpleFont font;
	
	library::SimpleFont& GUIRenderer::getFont()
	{
		return font;
	}
	
	void GUIRenderer::init(Renderer& renderer)
	{
		width  = 1.0;
		height = 1.0 / renderer.gamescr.SA;
		
		// orthographic projection
		ortho = orthoMatrix(width, height, 0, 2);
		
		// initialize our font renderer
		font.createTexture("bitmap/default/gui/font.png", 16);
		font.createShader();
		font.setClip(vec2(0.2, 0.0));
		
		// initialize inventories
		initInventoryRenderer();
		
		// initialize minimap
		minimap.init();
		
		// initialize chatbox & chat-transformation station
		chatbox.init(width, height);
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
		
		/// chatbox ///
		glEnable(GL_BLEND);
		
		vec2 textScale(0.01 * 0.6, 0.01);
		
		chatbox.render(font, ortho, textScale, renderer);
		
		/// debug text ///
		font.bind(0);
		font.setColor(vec4(0.8, 0.8, 1.0, 1.0));
		font.setBackColor(vec4(0.0, 0.4));
		
		font.print(vec3(0.01, 0.01, 0.0), textScale, "cppcraft v0.1", false);
		
		std::stringstream ss;
		ss << "fps: " << renderer.FPS;
		font.print(vec3(0.01, 0.02, 0.0), textScale, ss.str(), false);
		
		ss.str("");
		ss << "scene: " << renderer.scene_elements;
		font.print(vec3(0.01, 0.03, 0.0), textScale, ss.str(), false);
		
		glDisable(GL_BLEND);
	}
	
	
}
