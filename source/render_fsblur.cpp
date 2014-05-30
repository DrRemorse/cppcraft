#include "render_fs.hpp"

#include <library/opengl/opengl.hpp>
#include <library/opengl/vao.hpp>
#include "shaderman.hpp"
#include "textureman.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	extern VAO screenVAO;
	
	void FSRenderer::renderBlur(int blurw, int blurh)
	{
		Shader& blur = shaderman[Shaderman::BLUR];
		blur.bind();
		
		// gaussian blur:
		//static const double PI = 4 * atan(1);
		//const float sigma = 2.0;
		//blur.sendInteger("support", int(sigma * 3.0));
		//blur.sendVec2("sigma2", vec2(sigma * sigma, 1 / (sqrt(2 * PI) * sigma)));
		
		// regular blur:
		blur.sendInteger("Width", 3);
		
		// blur horizontally
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_BLURBUFFER1), 0);
		
		blur.sendVec2("dir", vec2(1.0 / blurw, 0.0));
		screenVAO.render(GL_QUADS);
		
		// blur vertically
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_BLURBUFFER2), 0);
		textureman.bind(0, Textureman::T_BLURBUFFER1);
		
		blur.sendVec2("dir", vec2(0.0, 1.0 / blurh));
		screenVAO.render(GL_QUADS);
	}
	
}
