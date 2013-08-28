#include "render_fs.hpp"

#include "library/opengl/opengl.hpp"
#include "library/opengl/window.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"

using namespace library;

namespace cppcraft
{
	void FSRenderer::renderBlur()
	{
		//startProgram(shaders.blurGaussian);
		//glUniform2f(shaders.blurGaussian.loc1, sigma * sigma, 1 / (sqrt(2 * PI) * sigma) );
		//glUniform1i(shaders.blurGaussian.loc2, int(sigma * 3.0) );
		
		//  blur horizontally (high)
		
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_BLURBUFFER1), 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
		
		Shader& blur = shaderman[Shaderman::BLUR];
		blur.bind();
		blur.sendInteger("Width", 2);
		blur.sendVec2("od", vec2(1.0 / blurTxW, 0.0));
		
		//  render
		glDrawArrays(GL_QUADS, 0, 4);
		
		//  blur vertically (high)
		
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_BLURBUFFER2), 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
		
		textureman.bind(0, Textureman::T_BLURBUFFER1);
		
		blur.sendVec2("od", vec2(0.0, 1.0 / blurTxH));
		
		//  render
		glDrawArrays(GL_QUADS, 0, 4);
		
		//  change back to normal
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
}
