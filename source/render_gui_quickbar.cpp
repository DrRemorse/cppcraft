#include "render_gui.hpp"

#include "library/bitmap/colortools.hpp"
#include "library/math/matrix.hpp"
#include "library/opengl/opengl.hpp"
#include "library/opengl/vao.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	VAO quickbarVAO;
	
	void GUIRenderer::renderQuickbar(Matrix& ortho, double frameCounter)
	{
		if (quickbarVAO.isGood() == false)
		{
			// create quickbar quad
			float sizex = this->width * 0.4;
			float sizey = sizex / 8;
			float x = (this->width - sizex) * 0.5;
			float y = this->height - sizey;
			
			GUIRenderer::gui_vertex_t vertices[4] =
			{
				{ x,         y,         0,   0, 0,   BGRA8(255, 255, 255, 0) },
				{ x + sizex, y,         0,   1, 0,   BGRA8(255, 255, 255, 0) },
				{ x + sizex, y + sizey, 0,   1, 1,   BGRA8(255, 255, 255, 0) },
				{ x,         y + sizey, 0,   0, 1,   BGRA8(255, 255, 255, 128) }
			};
			
			quickbarVAO.begin(sizeof (gui_vertex_t), 4, vertices);
			quickbarVAO.attrib(0, 3, GL_FLOAT, GL_FALSE, offsetof(gui_vertex_t, x));
			quickbarVAO.attrib(1, 2, GL_FLOAT, GL_FALSE, offsetof(gui_vertex_t, u));
			quickbarVAO.attrib(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(gui_vertex_t, color));
			quickbarVAO.end();
		}
		
		textureman.bind(0, Textureman::T_QUICKBAR);
		
		shaderman[Shaderman::GUI].bind();
		shaderman[Shaderman::GUI].sendMatrix("mvp", ortho);
		
		quickbarVAO.render(GL_QUADS);
		
	}
	
}
