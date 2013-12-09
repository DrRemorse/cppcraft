#include "fbo.hpp"

#include "../log.hpp"
#include "opengl.hpp"
#include "texture.hpp"

namespace library
{
	GLuint FBO::lastFBO = 0;
	
	FBO::FBO()
	{
		// initialize members
		fbo = 0;
	}
	
	void FBO::create()
	{
		// check for framebuffers support
		if (glGenFramebuffers == nullptr)
		{
			throw std::string("OpenGL was not initialized, or does not support framebuffers");
		}
		glGenFramebuffers(1, &fbo);
		if (fbo == 0) throw std::string("FBO was not generated, as the returned value was 0");
	}
	
	void FBO::bind()
	{
		if (fbo == 0) throw std::string("FBO was not yet created");
		
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		lastFBO = fbo;
	}
	
	void FBO::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		lastFBO = 0;
	}
	
	void FBO::attachColor(GLenum index, Texture& texture)
	{
		attachColor(index, texture.getHandle());
	}
	void FBO::attachColor(GLenum index, GLuint texture)
	{
		if (fbo == 0) throw std::string("FBO was not yet created");
		if (lastFBO != fbo) throw std::string("FBO was not currently bound, cannot attach color target");
		
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture, 0);
	}
	
	void FBO::removeColor(GLenum index)
	{
		if (fbo == 0) throw std::string("FBO was not yet created");
		if (lastFBO != fbo) throw std::string("FBO was not currently bound, cannot attach color target");
		
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, 0, 0);
	}
	
	void FBO::attachDepth(Texture& texture)
	{
		attachDepth(texture.getHandle());
	}
	void FBO::attachDepth(GLuint texture)
	{
		if (fbo == 0) throw std::string("FBO was not yet created");
		if (lastFBO != fbo) throw std::string("FBO was not currently bound, cannot attach color target");
		
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, texture, 0);
	}
	void FBO::removeDepth()
	{
		if (fbo == 0) throw std::string("FBO was not yet created");
		if (lastFBO != fbo) throw std::string("FBO was not currently bound, cannot attach color target");
		
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 0, 0);
	}
	
	void FBO::createDepthRBO(int width, int height)
	{
		// depth renderbuffer
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		// attach the rbo to this fbo
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "FBO::createDepthRBO(): Error creating depth renderbuffer" << Log::ENDL;
			throw std::string("FBO::createDepthRBO(): Error creating depth renderbuffer");
		}
	}
	void FBO::attachDepthRBO(const FBO& fboWithRBO)
	{
		rbo = fboWithRBO.rbo;
		// attach the rbo to this fbo
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "FBO::createDepthRBO(): Error creating depth renderbuffer" << Log::ENDL;
			throw std::string("FBO::createDepthRBO(): Error creating depth renderbuffer");
		}
	}
	
	void FBO::createMsaaRBO(int width, int height, int samples)
	{
		// depth renderbuffer
		GLuint rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		// attach the rbo to this fbo
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);
		
		if (isComplete() == false)
		{
			throw std::string(errorString());
		}
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "FBO::createMsaaRBO(): Error creating msaa renderbuffer" << Log::ENDL;
			throw std::string("FBO::createMsaaRBO(): Error creating msaa renderbuffer");
		}
	}
	
	void FBO::createDepthMsaaRBO(int width, int height, int samples)
	{
		// depth renderbuffer
		GLuint rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		// attach the rbo to this fbo
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
		
		if (isComplete() == false)
		{
			throw std::string(errorString());
		}
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "FBO::createDepthMsaaRBO(): Error creating msaa depth renderbuffer" << Log::ENDL;
			throw std::string("FBO::createDepthMsaaRBO(): Error creating msaa depth renderbuffer");
		}
	}
	
	void FBO::drawBuffers(std::vector<int> buffers)
	{
		glDrawBuffers(buffers.size(), (GLenum*) buffers.data());
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "FBO::drawBuffers(vector): Error setting draw buffers" << Log::ENDL;
			throw std::string("FBO::drawBuffers(vector): Error setting draw buffers");
		}
	}
	void FBO::drawBuffers()
	{
		GLenum one = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &one);
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "FBO::drawBuffers(): Error setting draw buffer" << Log::ENDL;
			throw std::string("FBO::drawBuffers(): Error setting draw buffer");
		}
	}
	
	void FBO::blitTo(FBO& dest_fbo, int w, int h, int mask, int filter)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest_fbo.getHandle());
		
		glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, mask, filter);
		
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
	
	bool FBO::isComplete()
	{
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		return status == GL_FRAMEBUFFER_COMPLETE;
	}
	
	std::string FBO::errorString()
	{
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			switch (status)
			{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
					return std::string("Incomplete attachment");
					
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
					return std::string("(Incomplete) Missing attachment");
					
				case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
					return std::string("(Incomplete) Layer targets");
					
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
					return std::string("Incomplete draw buffer");
					
				case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
					return std::string("Incomplete multisample format");
					
				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
					return std::string("Incomplete read buffer");
					
				case GL_FRAMEBUFFER_UNSUPPORTED:
					return std::string("Unsupported format");
					
			}
			return std::string("Framebuffer was incomplete, but unknown error");
		}
		return std::string("Framebuffer was complete (no error)");
	}
	
}
