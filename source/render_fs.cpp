#include "render_fs.hpp"

#include <library/config.hpp>
#include <library/log.hpp>
#include <library/opengl/opengl.hpp>
#include <library/opengl/fbo.hpp>
#include <library/opengl/vao.hpp>
#include <library/opengl/window.hpp>
#include <library/math/vector.hpp>
#include "camera.hpp"
#include "gameconf.hpp"
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
	FBO supersampler;
	
	void FSRenderer::init(WindowClass& gamescr)
	{
		logger << Log::INFO << "* Initializing screenspace renderer" << Log::ENDL;
		
		// create screenspace VAO
		screenVAO.createScreenspaceVAO();
		
		// set texture sizes
		// fs blur
		int factor = config.get("render.hq_blur", false) ? 1 : 2;
		this->blurTxW = gamescr.SW / factor;
		this->blurTxH = gamescr.SH / factor;
		
		// lens flare
		factor = config.get("render.hq_lens", false) ? 1 : 2;
		this->flareTxW = gamescr.SW / factor;
		this->flareTxH = gamescr.SH / factor;
		
		// create screenspace FBOs
		glGenFramebuffers(1, &blurFBO);
		supersampler.create();
		supersampler.bind();
		supersampler.attachColor(0, textureman[Textureman::T_RENDERBUFFER]);
		
		initFlare();
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "FSRenderer::init(): Failed to initialize framebuffers" << Log::ENDL;
			throw std::string("Failed to initialize screenspace framebuffers");
		}
	}
	
	void FSRenderer::blur(Texture& texture)
	{
		// downsize to blur-texture size
		glViewport(0, 0, blurTxW, blurTxH);
		// create blurred image from scene (current backbuffer image)
		renderBlur(texture.getWidth(), texture.getHeight());
		// upsize to regular screen size
		glViewport(0, 0, texture.getWidth(), texture.getHeight());
	}
	
	void FSRenderer::fog(vec3 playerPos, double timeElapsed)
	{
		Shader& shd = shaderman[Shaderman::FSTERRAINFOG];
		shd.bind();
		
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureman.get(Textureman::T_FOGBUFFER), 0);
		
		shd.sendVec3("sunAngle", thesun.getRealtimeAngle());
		// camera view matrix
		shd.sendMatrix("matview", camera.getViewMatrix());
		// player position
		shd.sendVec3("cameraPos", playerPos);
		// world offset for noise
		shd.sendVec3("worldOffset", camera.getWorldOffset());
		shd.sendFloat("timeElapsed", timeElapsed);
		
		// render fullscreen quad
		screenVAO.render(GL_QUADS);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void FSRenderer::terrain()
	{
		// postprocessing shader
		Shader& shd = shaderman[Shaderman::FSTERRAIN];
		shd.bind();
		
		// render fullscreen quad
		screenVAO.render(GL_QUADS);
	}
	
	void FSRenderer::renderSuperSampling(Texture& texture)	{
		Shader& shd = shaderman[Shaderman::SUPERSAMPLING];
		shd.bind();
		texture.bind(0);
		
		supersampler.bind();
		supersampler.attachColor(0, textureman[Textureman::T_RENDERBUFFER]);
		// downsample supersampling to screen size
		screenVAO.render(GL_QUADS);
		supersampler.unbind();
	}
	
	void FSRenderer::render(WindowClass& gamescr, double frameCounter)
	{
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		/////////////////////////////////
		// render sun flare
		/////////////////////////////////
		renderLensflare(gamescr);
		
		/////////////////////////////////
		/// fullscreen postprocessing ///
		/////////////////////////////////
		
		textureman.bind(0, Textureman::T_FINALBUFFER);
		textureman.bind(1, Textureman::T_LENSFLARE);
		
		// postprocessing shader
		Shader& shd = shaderman[Shaderman::POSTPROCESS];
		shd.bind();
		shd.sendFloat("frameCounter", frameCounter);
		shd.sendInteger("submerged", plogic.FullySubmerged);
		
		// render fullscreen quad
		screenVAO.render(GL_QUADS);
		
		#ifdef DEBUG
		if (ogl.checkError())
		{
			throw std::string("OpenGL state error after post process");
		}
		#endif
	}
	
}
