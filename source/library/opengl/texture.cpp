#include "texture.hpp"

#include "../log.hpp"
#include "../bitmap/bitmap.hpp"
#include "opengl.hpp"
#include <cmath>
#include <sstream>

namespace library
{
	GLuint Texture::lastid[TEXTURE_UNITS] = {0};
	GLenum Texture::lastUnit = 0;
	
	Texture::Texture() : id(0), type(0), format(0), boundUnit(0), isMipmapped(false) {}
	
	Texture::Texture(GLenum target)
	{
		this->type   = target;
		this->format = GL_RGBA;
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
	
	void Texture::create(const Bitmap& b, bool mipmap = true, GLint wrapmode = GL_CLAMP_TO_EDGE, GLint magfilter = GL_NEAREST, GLint minfilter = GL_LINEAR_MIPMAP_LINEAR)
	{
		bind(0);
		glTexParameteri(this->type, GL_TEXTURE_MAX_ANISOTROPY_EXT, ogl.anisotrophy);
		
		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, wrapmode);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, wrapmode);
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, magfilter);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, minfilter);
		
		// openGL is a C library, so const& is never going to work :)
		int width  = b.getwidth();
		int height = b.getheight();
		GLuint* pixel = b.data();
		int numTiles = b.getTilesX() * b.getTilesY();
		
		this->isMipmapped = mipmap;
		if (this->isMipmapped)
		{
			glTexParameteri(this->type, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(this->type, GL_TEXTURE_MAX_LEVEL, (int)(log(width) / log(2.0)));
		}
		
		switch (this->type)
		{
		case GL_TEXTURE_2D:
			glTexImage2D(this->type, 0, format, width, height, 0, GL_BGRA, ogl.storageformat, pixel);
			break;
		case GL_TEXTURE_CUBE_MAP:
			/* ====================================== */
			/* scope */;
			{
				// create temporary bitmap consisting of 1 side of a cubemap
				int cmsize = b.getwidth() / 4;
				Bitmap blitdump(cmsize, cmsize, 32);
				
				// iterate the 6 sides of a cubemap
				for (int i = 0; i < 6; i++)
				{
					switch (i)
					{
					// blit each side of the cube to our temporary bitmap at (0, 0)
					case 0: // +x
						b.blit(blitdump, cmsize * 2, cmsize * 1, cmsize, cmsize, 0, 0);
						break;
					case 1: // -x
						b.blit(blitdump, cmsize * 0, cmsize * 1, cmsize, cmsize, 0, 0);
						break;
					case 2: // +y
						b.blit(blitdump, cmsize * 1, cmsize * 0, cmsize, cmsize, 0, 0);
						break;
					case 3: // -y
						b.blit(blitdump, cmsize * 1, cmsize * 2, cmsize, cmsize, 0, 0);
						break;
					case 4: // +z
						b.blit(blitdump, cmsize * 1, cmsize * 1, cmsize, cmsize, 0, 0);
						break;
					case 5: // -z
						b.blit(blitdump, cmsize * 3, cmsize * 1, cmsize, cmsize, 0, 0);
						break;
					}
					// upload each side, from temporary bitmap
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, cmsize, cmsize, 0, GL_BGRA, ogl.storageformat, blitdump.data());
				}
			}
			/* ====================================== */
			break;
		case GL_TEXTURE_2D_ARRAY:
			glTexImage3D(this->type, 0, format, width, height, numTiles, 0, GL_BGRA, ogl.storageformat, pixel);
			break;
		default:
			logger << Log::ERR << "@Texture::create(): Unknown texture target (" << this->type << ")" << Log::ENDL;
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
		GLint minfilter = (mipmap) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
		
		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, minfilter);
		
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
			this->format = GL_DEPTH_COMPONENT24;
			glTexImage2D(this->type, 0, format, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
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
