#include "opengl.hpp"

#include "../log.hpp"
#include "fbo.hpp"
#include "window.hpp"

#include <string>

namespace library
{
	OpenGL ogl;
	
	OpenGL::OpenGL()
	{
		// standard 32-bits formats
		imageformat   = GL_RGBA8;
		storageformat = GL_UNSIGNED_BYTE;
	}
	
	// initialize glfw, open OpenGL window, read function entry points
	void OpenGL::init(WindowClass& window)
	{
		// set default viewport
		glViewport(0, 0, window.SW, window.SH);
		
		//-== openGL extensions ==-//
		
		//vertex buffer objects
		glGenBuffers    = (void(GLapi*)(GLsizei, GLuint*))glfwGetProcAddress("glGenBuffers");
		glBindBuffer    = (void(GLapi*)(GLenum, GLuint))glfwGetProcAddress("glBindBuffer");
		glBufferData    = (void(GLapi*)(GLenum, GLint, GLvoid*, GLenum))glfwGetProcAddress("glBufferData");
		glBufferSubData = (void(GLapi*)(GLenum, GLint, GLsizei, GLvoid*))glfwGetProcAddress("glBufferSubData");
		glDeleteBuffers = (void(GLapi*)(GLsizei, GLuint*))glfwGetProcAddress("glDeleteBuffers");
		
		//textures & mipmapping
		glGenerateMipmap	= (void(GLapi*)(GLenum))glfwGetProcAddress("glGenerateMipmap");
		glActiveTexture		= (void(GLapi*)(GLenum))glfwGetProcAddress("glActiveTexture");
		glTexImage3D		= (void(GLapi*)(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, GLvoid*))glfwGetProcAddress("glTexImage3D");
		glTexImage2DMultisample = (void(GLapi*)(GLenum, GLsizei, GLint, GLsizei, GLsizei, GLboolean))glfwGetProcAddress("glTexImage2DMultisample");
		
		// shader pipeline
		glCreateProgram			= (GLuint(GLapi*)())glfwGetProcAddress("glCreateProgram");
		glDeleteProgram	 		= (void(GLapi*)(GLuint))glfwGetProcAddress("glDeleteProgram");
		glCreateShader			= (GLuint(GLapi*)(GLenum))glfwGetProcAddress("glCreateShader");
		glDeleteShader			= (void(GLapi*)(GLuint))glfwGetProcAddress("glDeleteShader");
		glCompileShader			= (void(GLapi*)(GLuint))glfwGetProcAddress("glCompileShader");
		glShaderSource			= (void(GLapi*)(GLuint, GLsizei, GLchar**, GLint*))glfwGetProcAddress("glShaderSource");
		glGetShaderInfoLog		= (void(GLapi*)(GLuint, GLsizei, GLsizei*, GLchar*))glfwGetProcAddress("glGetShaderInfoLog");
		glGetShaderiv			= (void(GLapi*)(GLuint, GLenum, GLint*))glfwGetProcAddress("glGetShaderiv");
		glAttachShader          = (void(GLapi*)(GLuint, GLuint))glfwGetProcAddress("glAttachShader");
		glDetachShader          = (void(GLapi*)(GLuint, GLuint))glfwGetProcAddress("glDetachShader");
		glGetProgramiv			= (void(GLapi*)(GLuint, GLenum, GLint*))glfwGetProcAddress("glGetProgramiv");
		glGetProgramInfoLog		= (void(GLapi*)(GLuint, GLsizei, GLsizei*, GLchar*))glfwGetProcAddress("glGetProgramInfoLog");
		glLinkProgram			= (void(GLapi*)(GLuint))glfwGetProcAddress("glLinkProgram");
		glUseProgram			= (void(GLapi*)(GLuint))glfwGetProcAddress("glUseProgram");
		// shader uniforms
		glGetUniformLocation    = (GLint(GLapi*)(GLuint, GLchar*))glfwGetProcAddress("glGetUniformLocation");
		glUniform1i				= (void(GLapi*)(GLint, GLint))glfwGetProcAddress("glUniform1i");
		glUniform2i				= (void(GLapi*)(GLint, GLint, GLint))glfwGetProcAddress("glUniform2i");
		glUniform3i				= (void(GLapi*)(GLint, GLint, GLint, GLint))glfwGetProcAddress("glUniform3i");
		glUniform1f				= (void(GLapi*)(GLint, GLfloat))glfwGetProcAddress("glUniform1f");
		glUniform2f				= (void(GLapi*)(GLint, GLfloat, GLfloat))glfwGetProcAddress("glUniform2f");
		glUniform3f				= (void(GLapi*)(GLint, GLfloat, GLfloat, GLfloat))glfwGetProcAddress("glUniform3f");
		glUniform3fv			= (void(GLapi*)(GLint, GLsizei, GLfloat*))glfwGetProcAddress("glUniform3fv");
		glUniform4f				= (void(GLapi*)(GLint, GLfloat, GLfloat, GLfloat, GLfloat))glfwGetProcAddress("glUniform4f");
		glUniform4fv			= (void(GLapi*)(GLint, GLsizei, GLfloat*))glfwGetProcAddress("glUniform4fv");
		
		glUniformMatrix4fv      = (void(GLapi*)(GLint, GLsizei, GLboolean, GLfloat*))glfwGetProcAddress("glUniformMatrix4fv");
		
		// vertex array objects (VAOs)
		glGenVertexArrays           = (void(GLapi*)(GLsizei, GLuint*))glfwGetProcAddress("glGenVertexArrays");
		glBindVertexArray           = (void(GLapi*)(GLuint))glfwGetProcAddress("glBindVertexArray");
		glDeleteVertexArrays        = (void(GLapi*)(GLsizei, GLuint*))glfwGetProcAddress("glDeleteVertexArrays");
		
		glDisableVertexAttribArray	= (void(GLapi*)(GLuint))glfwGetProcAddress("glDisableVertexAttribArray");
		glEnableVertexAttribArray	= (void(GLapi*)(GLuint))glfwGetProcAddress("glEnableVertexAttribArray");
												//	index,  count,   type,  normalized, stride,  data
		glVertexAttribPointer		= (void(GLapi*)(GLuint, GLsizei, GLenum, GLboolean, GLsizei, GLvoid*))glfwGetProcAddress("glVertexAttribPointer");
		glBindAttribLocation        = (void(GLapi*)(GLuint, GLuint, GLchar*))glfwGetProcAddress("glBindAttribLocation");
		
		// occlusion queries
		glGenQueries                = (void(GLapi*)(GLsizei, GLuint*))glfwGetProcAddress("glGenQueries");
		glDeleteQueries             = (void(GLapi*)(GLsizei, GLuint*))glfwGetProcAddress("glDeleteQueries");
		glGetQueryObjectuiv         = (void(GLapi*)(GLuint, GLenum, GLuint*))glfwGetProcAddress("glGetQueryObjectuiv");
		glBeginQuery				= (void(GLapi*)(GLenum, GLuint))glfwGetProcAddress("glBeginQuery");
		glEndQuery					= (void(GLapi*)(GLenum))glfwGetProcAddress("glEndQuery");
		
		// point sprites
		//glPointParameterfARB        = (void(GLapi*)())glfwGetProcAddress("glPointParameterf")
		//glPointParameterfvARB       = (void(GLapi*)())glfwGetProcAddress("glPointParameterfv")
		
		// framebuffers
		glGenFramebuffers		= (void(GLapi*) (GLsizei count, GLuint* ids))glfwGetProcAddress("glGenFramebuffers");
		glDeleteFramebuffers 	= (void(GLapi*) (GLsizei count, GLuint* ids))glfwGetProcAddress("glDeleteFramebuffers");
		glBindFramebuffer 		= (void(GLapi*) (GLenum bufferType, GLuint fbo))glfwGetProcAddress("glBindFramebuffer");
		glFramebufferTexture2D 	= (void(GLapi*) (GLenum, GLenum, GLenum, GLuint, GLint))glfwGetProcAddress("glFramebufferTexture2D");
		glFramebufferTexture 	= (void(GLapi*) (GLenum, GLenum, GLuint, GLint))glfwGetProcAddress("glFramebufferTexture");
		glBlitFramebuffer 		= (void(GLapi*) (GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLenum))
												glfwGetProcAddress("glBlitFramebuffer");
		glCheckFramebufferStatus = (GLenum(GLapi*) (GLenum bufferType))glfwGetProcAddress("glCheckFramebufferStatus");
		glDrawBuffers           = (void(GLapi*) (GLsizei count, GLenum* buffers))glfwGetProcAddress("glDrawBuffers");
		// renderbuffers
		glGenRenderbuffers 		= (void(GLapi*) (GLsizei count, GLuint* rboIDs))glfwGetProcAddress("glGenRenderbuffers");
		glBindRenderbuffer 		= (void(GLapi*) (GLenum bufferType, GLuint rbo))glfwGetProcAddress("glBindRenderbuffer");
		glRenderbufferStorage 	= (void(GLapi*) (GLenum, GLenum, GLsizei, GLsizei))glfwGetProcAddress("glRenderbufferStorage");
		glRenderbufferStorageMultisample = 	(void(GLapi*) (GLenum, GLsizei, GLenum, GLsizei, GLsizei))
												glfwGetProcAddress("glRenderbufferStorageMultisample");
		glFramebufferRenderbuffer = (void(GLapi*) (GLenum, GLenum, GLenum, GLuint))glfwGetProcAddress("glFramebufferRenderbuffer");
		
		if (glGenBuffers == nullptr)
		{
			logger << Log::ERR << "Your video card does not support VBO's (OpenGL 1.2+). Exiting." << Log::ENDL;
			throw std::string("Opengl::init(): Missing VBO support, check your drivers!");
		}
		if (glCreateProgram == nullptr)
		{
			logger << Log::ERR << "Your video card does not support Programmable Shader Pipeline. Exiting." << Log::ENDL;
			throw std::string("Opengl::init(): Missing shader support, check your drivers!");
		}
		if (glGenVertexArrays == nullptr)
		{
			logger << Log::ERR << "Your video card does not support Vertex Array Objects. Exiting." << Log::ENDL;
			throw std::string("Opengl::init(): Missing VAO support, check your drivers!");
		}
		if (glVertexAttribPointer == nullptr)
		{
			logger << Log::ERR << "Your video card does not support Vertex Attributes. Exiting." << Log::ENDL;
			throw std::string("Opengl::init(): Missing vertexattrib support, check your drivers!");
		}
		
		if (glGenerateMipmap == nullptr)
		{
			logger << Log::ERR << "Your video card does not support automatic mipmap generation. Exiting." << Log::ENDL;
			throw std::string("Opengl::init(): Missing mipmap generation support, check your drivers!");
		}
		
		// default states
		// GL_COMPRESSED_RGBA setting
		glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
		// GL_GENERATE_MIPMAP setting
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		// perspective-correct interpolation setting
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		
		// cleared colorbuffer value
		glClearColor(0.0, 0.0, 0.0, 0.0);
		// cleared depth values
		glClearDepth(1.0);
		// full depth range
		glDepthRange(0.0, 1.0);
		
		// default backface culling & fulfilled shading
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT, GL_FILL);
		
		// default blend function
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		GLenum glerr = glGetError();
		if (glerr)
		{
			logger << Log::ERR << "OpenGL initialization error: " << glerr << Log::ENDL;
			throw std::string("OpenGL::init(): Initialization error");
		}
		
	}
	
	const bool OpenGL::checkError()
	{
		GLenum error = glGetError();
		if (error)
		{
			std::string errorString;
			switch (error)
			{
			case GL_INVALID_ENUM:
				errorString = "Invalid enumeration parameter (GL_INVALID_ENUM)";
				break;
			case GL_INVALID_VALUE:
				errorString = "Invalid value for parameter (GL_INVALID_VALUE)";
				break;
			case GL_INVALID_OPERATION:
				errorString = "Invalid operation for current state (GL_INVALID_OPERATION)";
				break;
				
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorString = "Invalid operation on incomplete framebuffer (GL_INVALID_FRAMEBUFFER_OPERATION)";
				errorString += "\nFramebuffer error: " + FBO::errorString();
				break;
				
			default:
				errorString = "Unknown error";
			}
			
			logger << Log::ERR << "OpenGL error(" << error << "): " << errorString << Log::ENDL;
			return true;
		}
		return false;
	}
	
}
