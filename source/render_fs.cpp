#include "render_fs.hpp"

#include "library/opengl/opengl.hpp"
#include "library/opengl/vao.hpp"
#include "library/opengl/window.hpp"
#include "player_logic.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"

using namespace library;

namespace cppcraft
{
	FSRenderer screenspace;
	VAO screenVAO;
	
	void FSRenderer::init(WindowClass& gamescr)
	{
		// create screenspace VAO
		screenVAO.createScreenspaceVAO();
		
		// set texture sizes
		this->blurTxW = gamescr.SW / 2;
		this->blurTxH = gamescr.SH / 2;
		
		this->flareTxW = gamescr.SW / 2;
		this->flareTxH = gamescr.SH / 2;
		
		// create screenspace FBOs
		glGenFramebuffers(1, &blurFBO);
		glGenFramebuffers(1, &flareFBO);
		
		initFlare();
	}
	
	void FSRenderer::blur(WindowClass& gamescr)
	{
		// downsize to blur-texture size
		glViewport(0, 0, blurTxW, blurTxH);
		// create blurred image from scene (current backbuffer image)
		renderBlur();
		// upsize to regular screen size
		glViewport(0, 0, gamescr.SW, gamescr.SH);
	}
	
	void FSRenderer::terrain(WindowClass& gamescr)
	{
		textureman.bind(0, Textureman::T_RENDERBUFFER);
		textureman.bind(1, Textureman::T_DEPTHBUFFER);
		textureman.bind(2, Textureman::T_BLURBUFFER2);
		textureman.bind(3, Textureman::T_SKYBUFFER);
		
		// postprocessing shader
		Shader& shd = shaderman[Shaderman::FSTERRAIN];
		shd.bind();
		
		// render fullscreen quad
		screenVAO.render(GL_QUADS);
	}
	
	void FSRenderer::render(WindowClass& gamescr)
	{
		//  render sun sphere and check samples passed using GL_LEQUAL
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		// copy the current screen buffer
		textureman.bind(0, Textureman::T_RENDERBUFFER);
		textureman.copyScreen(gamescr, Textureman::T_RENDERBUFFER);
		
		glDisable(GL_BLEND);
		
		// render sun flare
		renderLensflare(gamescr);
		
		/// fullscreen postprocessing ///
		
		textureman.bind(0, Textureman::T_RENDERBUFFER);
		textureman.bind(1, Textureman::T_DEPTHBUFFER);
		textureman.bind(2, Textureman::T_LENSFLARE);
		
		// postprocessing shader
		Shader& shd = shaderman[Shaderman::POSTPROCESS];
		shd.bind();
		shd.sendInteger("submerged", plogic.FullySubmerged);
		
		// render fullscreen quad
		screenVAO.render(GL_QUADS);
		
		if (ogl.checkError()) throw std::string("Error after post process");
	}
	
}
