#include "render_fs.hpp"

#include "library/opengl/opengl.hpp"
#include "library/opengl/vao.hpp"
#include "library/opengl/window.hpp"
#include "library/math/vector.hpp"
#include "player.hpp"
#include "player_logic.hpp"
#include "shaderman.hpp"
#include "sun.hpp"
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
		this->blurTxW = gamescr.SW;
		this->blurTxH = gamescr.SH;
		
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
	
	void FSRenderer::fog(WindowClass& gamescr)
	{
		Shader& shd = shaderman[Shaderman::FSTERRAINFOG];
		shd.bind();
		
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_RENDERBUFFER), 0);
		
		// dot for how much we are looking at sun
		shd.sendFloat("daylight", thesun.getRealtimeDaylight());
		
		float sundot = thesun.getRealtimeAngle().dot(player.getLookVector());
		shd.sendFloat("sundot", sundot);
		
		vec2 sunpos = getSunVector(thesun.getSunMatrix());
		shd.sendVec2("sunCoord", sunpos);
		
		// render fullscreen quad
		screenVAO.render(GL_QUADS);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void FSRenderer::terrain(WindowClass& gamescr)
	{
		textureman.bind(0, Textureman::T_RENDERBUFFER);
		textureman.bind(1, Textureman::T_BLURBUFFER2);
		
		// postprocessing shader
		Shader& shd = shaderman[Shaderman::FSTERRAIN];
		shd.bind();
		
		// render fullscreen quad
		screenVAO.render(GL_QUADS);
	}
	
	void FSRenderer::render(WindowClass& gamescr)
	{
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		// render sun flare
		renderLensflare(gamescr);
		
		/// fullscreen postprocessing ///
		
		textureman.bind(0, Textureman::T_FOGBUFFER);
		textureman.bind(1, Textureman::T_LENSFLARE);
		
		// postprocessing shader
		Shader& shd = shaderman[Shaderman::POSTPROCESS];
		shd.bind();
		shd.sendInteger("submerged", plogic.FullySubmerged);
		
		// render fullscreen quad
		screenVAO.render(GL_QUADS);
		
		if (ogl.checkError()) throw std::string("Error after post process");
	}
	
}
