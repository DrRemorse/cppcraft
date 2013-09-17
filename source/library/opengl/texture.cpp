#include "texture.hpp"

#include "../log.hpp"
#include "../bitmap/bitmap.hpp"
#include "opengl.hpp"
#include <cmath>
#include <sstream>

namespace library
{
	GLuint Texture::lastid[TEXTURE_UNITS] = {0};
	GLenum Texture::lastUnit = -1;
	
	Texture::Texture() : id(0), type(0), format(0), boundUnit(0), isMipmapped(false) {}
	
	Texture::Texture(GLenum target)
	{
		this->type   = target;
		this->format = GL_RGBA8;
		glGenTextures(1, &this->id);
		this->boundUnit = 0;
		this->isMipmapped = false;
	}
	
	Texture::Texture(GLenum target, GLint format)
	{
		this->type   = target;
		this->format = format;
		glGenTextures(1, &this->id);
		this->boundUnit = 0;
		this->isMipmapped = false;
	}
	
	void Texture::setFormat(GLint newFormat)
	{
		this->format = newFormat;
	}
	
	void Texture::create(const Bitmap& bmp, bool mipmap = true, GLint wrapmode = GL_CLAMP_TO_EDGE, GLint magfilter = GL_NEAREST, GLint minfilter = GL_LINEAR_MIPMAP_LINEAR)
	{
		bind(0);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, wrapmode);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, wrapmode);
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, magfilter);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, minfilter);
		
		// openGL is a C library, so const& is never going to work :)
		this->width  = bmp.getwidth();
		this->height = bmp.getheight();
		GLuint* pixel = bmp.data();
		
		this->isMipmapped = mipmap;
		if (this->isMipmapped)
		{
			glTexParameteri(this->type, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(this->type, GL_TEXTURE_MAX_LEVEL, (int)(log(width) / log(2.0)));
		}
		
		if (this->type == GL_TEXTURE_2D)
		{
			glTexImage2D(this->type, 0, format, width, height, 0, bmp.getFormat(), ogl.storageformat, pixel);
		}
		else if (this->type == GL_TEXTURE_CUBE_MAP)
		{
			/* ====================================== */
			
			// create temporary bitmap consisting of 1 side of a cubemap
			int cmsize = width / 4;
			Bitmap blitdump(cmsize, cmsize, 32);
			
			// iterate the 6 sides of a cubemap
			for (int i = 0; i < 6; i++)
			{
				switch (i)
				{
				// blit each side of the cube to our temporary bitmap at (0, 0)
				case 0: // +x
					bmp.blit(blitdump, cmsize * 2, cmsize * 1, cmsize, cmsize, 0, 0);
					break;
				case 1: // -x
					bmp.blit(blitdump, cmsize * 0, cmsize * 1, cmsize, cmsize, 0, 0);
					break;
				case 2: // +y
					bmp.blit(blitdump, cmsize * 1, cmsize * 0, cmsize, cmsize, 0, 0);
					break;
				case 3: // -y
					bmp.blit(blitdump, cmsize * 1, cmsize * 2, cmsize, cmsize, 0, 0);
					break;
				case 4: // +z
					bmp.blit(blitdump, cmsize * 1, cmsize * 1, cmsize, cmsize, 0, 0);
					break;
				case 5: // -z
					bmp.blit(blitdump, cmsize * 3, cmsize * 1, cmsize, cmsize, 0, 0);
					break;
				}
				// upload each side, from temporary bitmap
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, cmsize, cmsize, 0, bmp.getFormat(), ogl.storageformat, blitdump.data());
			}
			/* ====================================== */
		}
		else if (this->type == GL_TEXTURE_2D_ARRAY)
		{
			int numTiles = bmp.getTilesX() * bmp.getTilesY();
			
			glTexImage3D(this->type, 0, format, width, height, numTiles, 0, bmp.getFormat(), ogl.storageformat, pixel);
		}
		else
		{
			logger << Log::ERR << "@Texture::create(): Unknown texture target (" << (int)this->type << ")" << Log::ENDL;
		}
		
		if (this->isMipmapped)
		{
			glGenerateMipmap(this->type);
		}
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "Texture::create(): OpenGL state error" << Log::ENDL;
			logger << Log::ERR << toString() << Log::ENDL;
			throw std::string("Texture::create(): OpenGL state error");
		}
	}
	
	void Texture::create(bool mipmap, int levels, int width, int height)
	{
		bind(0);
		const GLint minfilter = (mipmap) ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST;
		const GLint maxfilter = (mipmap) ? GL_LINEAR : GL_NEAREST;
		
		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, maxfilter);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, minfilter);
		
		this->width  = width;
		this->height = height;
		
		this->isMipmapped = mipmap;
		if (this->isMipmapped)
		{
			glTexParameteri(this->type, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(this->type, GL_TEXTURE_MAX_LEVEL, levels);
		}
		
		GLenum sformat = ogl.storageformat;
		if (format == GL_RGBA16F_ARB || format == GL_RGBA32F_ARB) sformat = GL_FLOAT;
		
		switch (this->type)
		{
		case GL_TEXTURE_2D:
			glTexImage2D(this->type, 0, format, width, height, 0, GL_BGRA, sformat, nullptr);
			break;
		default:
			logger << Log::ERR << "Texture::create(): Unknown texture target (" << this->type << ")" << Log::ENDL;
			logger << Log::ERR << toString();
			throw std::string("Failed to create texture");
		}
		
		if (this->isMipmapped)
		{
			glGenerateMipmap(this->type);
		}
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "Texture::create(): OpenGL state error" << Log::ENDL;
			logger << Log::ERR << toString() << Log::ENDL;
			throw std::string("Texture::create(): OpenGL state error");
		}
	}
	
	void Texture::setInterpolation(bool linear)
	{
		GLint maxfilter = (linear) ? GL_LINEAR : GL_NEAREST;
		GLint minfilter = maxfilter;
		if (isMipmapped) minfilter = (linear) ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST;
		
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, maxfilter);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, minfilter);
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "Texture::setInterpolation(): OpenGL state error" << Log::ENDL;
			logger << Log::ERR << toString() << Log::ENDL;
			throw std::string("Texture::setInterpolation(): OpenGL state error");
		}
	}
	void Texture::setWrapMode(GLint wrapmode)
	{
		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, wrapmode);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, wrapmode);
	}
	void Texture::setAnisotropy(float samples)
	{
		glTexParameterf(this->type, GL_TEXTURE_MAX_ANISOTROPY_EXT, samples);
	}
	
	void Texture::createMultisample(int numsamples, int width, int height)
	{
		// set type before binding
		this->type = GL_TEXTURE_2D_MULTISAMPLE;
		
		bind(0);
		this->width  = width;
		this->height = height;
		this->format = GL_RGBA;
		this->isMipmapped = false;
		
		glTexImage2DMultisample(this->type, numsamples, this->format, width, height, GL_TRUE);
		
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "Texture::createMultisample(): OpenGL state error" << Log::ENDL;
			logger << Log::ERR << toString() << Log::ENDL;
			throw std::string("Texture::createMultisample(): OpenGL state error");
		}
	}
	
	void Texture::createDepth(bool stencil24d8s, int width, int height)
	{
		bind(0);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// create empty depth buffer texture
		this->width  = width;
		this->height = height;
		
		if (stencil24d8s)
		{
			this->format = GL_DEPTH24_STENCIL8;
			glTexImage2D(this->type, 0, format, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		}
		else
		{
			// let opengl give the highest supported for this context
			this->format = GL_DEPTH_COMPONENT;
			glTexImage2D(this->type, 0, format, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);
		}
		if (OpenGL::checkError())
		{
			logger << Log::ERR << "Texture::createDepth(): OpenGL state error" << Log::ENDL;
			logger << Log::ERR << toString() << Log::ENDL;
			throw std::string("Texture::createDepth(): OpenGL state error");
		}
	}
	
	void Texture::bind(GLenum unit)
	{
		// avoid binding same texture twice on the same texture unit
		if (lastid[unit] == this->id) return;
		lastid[unit] = this->id;
		
		if (lastUnit != unit)
		{
			lastUnit = unit;
			glActiveTexture(GL_TEXTURE0 + unit);
		}
		
		glBindTexture(this->type, this->id);
		this->boundUnit = unit;
		
		if (OpenGL::checkError())
			throw std::string("Texture::bind(): OpenGL state error");
	}
	
	void Texture::unbind()
	{
		unbind(boundUnit);
	}
	
	void Texture::unbind(GLenum unit)
	{
		if (lastUnit != unit)
		{
			lastUnit = unit;
			glActiveTexture(GL_TEXTURE0 + unit);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		lastid[unit] = 0;
	}
	
	void Texture::copyScreen(int w, int h)
	{
		// explicitly select textures last texture unit
		if (lastUnit != getBoundUnit())
		{
			glActiveTexture(GL_TEXTURE0 + getBoundUnit());
			lastUnit = getBoundUnit();
		}
		// copy screen
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, w, h);
	}
	
	void Texture::uploadBGRA8(const Bitmap& bmp)
	{
		this->width  = bmp.getwidth();
		this->height = bmp.getheight();
		
		// explicitly set active texture unit
		glActiveTexture(GL_TEXTURE0 + lastUnit);
		// upload pixel data
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bmp.data());
		
		// auto-generate new mipmap levels
		if (this->isMipmapped)
		{
			glGenerateMipmap(this->type);
		}
	}
	
	GLuint Texture::getHandle()
	{
		return this->id;
	}
	
	GLenum Texture::getBoundUnit() const
	{
		return this->boundUnit;
	}
	
	std::string Texture::toString() const
	{
		std::string typeString;
		std::string formatString;
		
		switch (type)
		{
		case GL_TEXTURE_1D:
			typeString = "GL_TEXTURE_1D"; break;
		case GL_TEXTURE_2D:
			typeString = "GL_TEXTURE_2D"; break;
		case GL_TEXTURE_2D_MULTISAMPLE:
			typeString = "GL_TEXTURE_2D_MULTISAMPLE"; break;
		case GL_TEXTURE_2D_ARRAY:
			typeString = "GL_TEXTURE_2D_ARRAY"; break;
		case GL_TEXTURE_3D:
			typeString = "GL_TEXTURE_3D"; break;
		case GL_TEXTURE_CUBE_MAP:
			typeString = "GL_TEXTURE_CUBE_MAP"; break;
		default:
			typeString = "(Unknown type)"; break;
		}
		
		switch (format)
		{
		case GL_RGBA:
			formatString = "GL_RGBA"; break;
		case GL_RGBA16F_ARB:
			formatString = "GL_RGBA16F"; break;
		case GL_RGBA32F_ARB:
			formatString = "GL_RGBA32F"; break;
		case GL_COMPRESSED_RGBA:
			formatString = "GL_COMPRESSED_RGBA"; break;
		case GL_DEPTH_COMPONENT:
			formatString = "GL_DEPTH_COMPONENT"; break;
		case GL_DEPTH_COMPONENT16:
			formatString = "GL_DEPTH_COMPONENT16"; break;
		case GL_DEPTH_COMPONENT24:
			formatString = "GL_DEPTH_COMPONENT24"; break;
		case GL_DEPTH_COMPONENT32:
			formatString = "GL_DEPTH_COMPONENT32"; break;
		case GL_DEPTH24_STENCIL8:
			formatString = "GL_DEPTH24_STENCIL8"; break;
		default:
			formatString = "(Unknown format)"; break;
		}
		
		std::stringstream ss;
		ss << "Texture ID: " << id << " Size: (" << width << ", " << height << ") Type: " + typeString + " Format: " + formatString;
		return ss.str();
	}
	
}
