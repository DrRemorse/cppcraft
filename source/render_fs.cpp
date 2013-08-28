#include "render_fs.hpp"

#include "library/opengl/opengl.hpp"
#include "library/opengl/window.hpp"
#include "player_logic.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"

using namespace library;

namespace cppcraft
{
	FSRenderer screenspace;
	
	void FSRenderer::init(WindowClass& gamescr)
	{
		// create screenspace VAO
		glGenVertexArrays(1, &screenVAO);
		glBindVertexArray(screenVAO);
		
		struct screenvertex_t
		{
			float x, y, z;
		};
		
		screenvertex_t sv_t[4];
		sv_t[0] = (screenvertex_t) {0, 0, 0};
		sv_t[1] = (screenvertex_t) {1, 0, 0};
		sv_t[2] = (screenvertex_t) {1, 1, 0};
		sv_t[3] = (screenvertex_t) {0, 1, 0};
		
		GLuint vbo = 0;
		glGenBuffers(1, &vbo);
		
		// vertices
		glBindBuffer(GL_ARRAY_BUFFER_ARB, vbo);
		glBufferData(GL_ARRAY_BUFFER_ARB, 4 * sizeof(screenvertex_t), sv_t, GL_STATIC_DRAW_ARB);
		
		// vertex data format
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(screenvertex_t), 0);
		
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
		glDisableVertexAttribArray(0);
		
		// set texture sizes
		this->blurTxW = gamescr.SW;
		this->blurTxH = gamescr.SH;
		
		this->flareTxW = gamescr.SW / 2;
		this->flareTxH = gamescr.SH / 2;
		
		// create screenspace FBOs
		glGenFramebuffers(1, &blurFBO);
		glGenFramebuffers(1, &flareFBO);
		
	}
	
	void FSRenderer::blur(WindowClass& gamescr)
	{
		// bind screenspace VAO
		glBindVertexArray(screenVAO);
		
		; //if (renderconf.highq_blur)
		{
			// downsize to blur-texture size
			glViewport(0, 0, blurTxW, blurTxH);
			// create blurred image from scene (current backbuffer image)
			renderBlur();
			// upsize to regular screen size
			glViewport(0, 0, gamescr.SW, gamescr.SH);
		}
		
		glBindVertexArray(0);
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
		glBindVertexArray(screenVAO);
		glDrawArrays(GL_QUADS, 0, 4);
		
		// unbind screenspace VAO
		glBindVertexArray(0);
	}
	
	void FSRenderer::render(WindowClass& gamescr)
	{
		glBindVertexArray(0);
		
		//textureman.bind(1, Textureman::T_DEPTHBUFFER);
		//textureman.copyScreen(gamescr, Textureman::T_DEPTHBUFFER);
		
		// copy the current screen buffer
		textureman.bind(0, Textureman::T_RENDERBUFFER);
		textureman.copyScreen(gamescr, Textureman::T_RENDERBUFFER);
		//glGenerateMipmap(GL_TEXTURE_2D);
		
		glEnable(GL_BLEND);
		
		; //if (renderconf.lensflare)
		{
			// render sun flare
			renderLensflare(gamescr);
		}
		
		// clear buffers
		// glClear(GL_DEPTH_BUFFER_BIT)
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST); // mui importante!
		glDepthMask(GL_FALSE);
		
		/*
		// bind screenspace VAO
		glBindVertexArray(screenVAO);
		
		; //if (renderconf.highq_blur)
		{
			// downsize to blur-texture size
			glViewport(0, 0, blurTxW, blurTxH);
			// bind texture containing scene
			textureman.bind(0, Textureman::T_RENDERBUFFER);
			// create blurred image from scene (current backbuffer image)
			renderBlur();
			// upsize to regular screen size
			glViewport(0, 0, gamescr.SW, gamescr.SH);
		}
		*/
		
		textureman.bind(0, Textureman::T_RENDERBUFFER);
		textureman.bind(1, Textureman::T_DEPTHBUFFER);
		textureman.bind(2, Textureman::T_LENSFLARE);
		
		// postprocessing shader
		Shader& shd = shaderman[Shaderman::POSTPROCESS];
		shd.bind();
		shd.sendInteger("submerged", plogic.FullySubmerged);
		
		// render fullscreen quad
		glBindVertexArray(screenVAO);
		glDrawArrays(GL_QUADS, 0, 4);
		
		// unbind screenspace VAO
		glBindVertexArray(0);
		
		if (ogl.checkError()) throw std::string("Error after post process");
	}
	
}
