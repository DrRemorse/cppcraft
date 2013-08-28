#include "render_fs.hpp"

#include "library/opengl/opengl.hpp"
#include "library/opengl/window.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	void FSRenderer::renderBlur()
	{
		const double PI = 4 * atan(1);
		const int BLUR_LEVEL = 2;
		const float sigma = 1.0;
		
		Shader& blur = shaderman[Shaderman::BLUR];
		blur.bind();
		
		// gaussian blur:
		blur.sendInteger("support", int(sigma * 3.0));
		blur.sendVec2("sigma2", vec2(sigma * sigma, 1 / (sqrt(2 * PI) * sigma)));
		
		// regular blur:
		//blur.sendInteger("Width", BLUR_LEVEL);
		
		// blur horizontally
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_BLURBUFFER1), 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
		
		blur.sendVec2("dir", vec2(1.0 / blurTxW, 0.0));
		
		// render
		glDrawArrays(GL_QUADS, 0, 4);
		
		// blur vertically
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_BLURBUFFER2), 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
		
		textureman.bind(0, Textureman::T_BLURBUFFER1);
		
		blur.sendVec2("dir", vec2(0.0, 1.0 / blurTxH));
		
		// render
		glDrawArrays(GL_QUADS, 0, 4);
		
		// change back to normal
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
}
