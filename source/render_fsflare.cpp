#include "render_fs.hpp"

#include "library/log.hpp"
#include "library/opengl/opengl.hpp"
#include "library/opengl/window.hpp"
#include "frustum.hpp"
#include "render_sky.hpp"
#include "sun.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"

using namespace library;

namespace cppcraft
{
	
	void FSRenderer::renderLensflare(WindowClass& gamescr)
	{
		//  render sun sphere and check samples passed using GL_LEQUAL
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);
		
		//  render sun to texture 0
		glBindFramebuffer(GL_FRAMEBUFFER, flareFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_LENSFLARE), 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
		
		//  need to clear buffer before we exit, so that the lensflare buffer is valid!
		glViewport(0, 0, flareTxW, flareTxH);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//  render sun (again) to texture 0
		Matrix matsun = skyrenderer.renderSunProj(1.0);
		
		glDisable(GL_DEPTH_TEST);
		
		//  screenspace VAO
		glBindVertexArray(screenVAO);
		
		//  LOW blur sun sphere horizontally (low)
		glBindFramebuffer(GL_FRAMEBUFFER, flareFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_LENSFLARE3), 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
		
		textureman.bind(0, Textureman::T_LENSFLARE);
		
		Shader& blur = shaderman[Shaderman::BLUR];
		blur.bind();
		blur.sendInteger("Width", 2);
		blur.sendVec2("dir", vec2(1.0 / flareTxW, 0.0));
		
		//  render
		glDrawArrays(GL_QUADS, 0, 4);
		
		//  LOW blur sun sphere vertically (low) (final result = texture 1)
		
		glBindFramebuffer(GL_FRAMEBUFFER, flareFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_LENSFLARE2), 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
		
		textureman.bind(0, Textureman::T_LENSFLARE3);
		
		blur.sendVec2("dir", vec2(0.0, 1.0 / flareTxH));
		
		//  render
		glDrawArrays(GL_QUADS, 0, 4);
		
		textureman.unbind(0);
		
		//  HIGH blur sun sphere horizontally (high)
		
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_LENSFLARE), 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
		
		textureman.bind(0, Textureman::T_LENSFLARE2);
		
		blur.sendInteger("Width", 6);
		blur.sendVec2("dir", vec2(1.0 / flareTxW, 0.0));
		
		//  render
		glDrawArrays(GL_QUADS, 0, 4);
		
		// free texture unit
		textureman.unbind(0);
		
		//  blur sun sphere vertically (high) (final result = 0)
		
		glBindFramebuffer(GL_FRAMEBUFFER, flareFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_LENSFLARE3), 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
		
		textureman.bind(0, Textureman::T_LENSFLARE);
		
		blur.sendVec2("dir", vec2(0.0, 1.0 / flareTxH));
		
		//  render
		glDrawArrays(GL_QUADS, 0, 4);
		
		// free texture unit
		textureman.unbind(0);
		
		//  blur sun sphere radially, calculate lens flare and halo
		
		glBindFramebuffer(GL_FRAMEBUFFER, flareFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_LENSFLARE), 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
		
		textureman.bind(2, Textureman::T_LENSDIRT);
		textureman.bind(1, Textureman::T_LENSFLARE3);
		textureman.bind(0, Textureman::T_LENSFLARE2);
		
		Matrix matbias;
		matbias.bias();
		
		vec4 sunproj = (matbias * frustum.getProjection() * matsun) * vec4(0.0, 0.0, 0.0, 1.0);
		
		Shader& lens = shaderman[Shaderman::LENSFLARE];
		lens.bind();
		lens.sendVec4("sunproj", sunproj);
		
		//  render final lens image
		glDrawArrays(GL_QUADS, 0, 4);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		// restore viewport
		glViewport(0, 0, gamescr.SW, gamescr.SH);
		
		if (ogl.checkError())
			throw std::string("Error after rendering lens flare");
	}
}
