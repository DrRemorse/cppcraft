#include "render_fs.hpp"

#include "library/opengl/opengl.hpp"
#include "library/opengl/vao.hpp"
#include "library/opengl/window.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	extern VAO screenVAO;
	
	void FSRenderer::renderBlur()
	{
		Shader& blur = shaderman[Shaderman::GAUSS];
		blur.bind();
		
		// gaussian blur:
		const double PI = 4 * atan(1);
		const float sigma = 1.5;
		blur.sendInteger("support", int(sigma * 3.0));
		blur.sendVec2("sigma2", vec2(sigma * sigma, 1 / (sqrt(2 * PI) * sigma)));
		
		// regular blur:
		//const int BLUR_LEVEL = 3;
		//blur.sendInteger("Width", BLUR_LEVEL);
		
		// blur horizontally
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_BLURBUFFER1), 0);
		
		blur.sendVec2("dir", vec2(1.0 / blurTxW, 0.0));
		
		// render
		screenVAO.render(GL_QUADS);
		
		// blur vertically
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_BLURBUFFER2), 0);
		
		textureman.bind(0, Textureman::T_BLURBUFFER1);
		
		blur.sendVec2("dir", vec2(0.0, 1.0 / blurTxH));
		
		// render
		screenVAO.render(GL_QUADS);
		
		// change back to normal
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
}
