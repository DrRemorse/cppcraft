#include "bitmap.hpp"

#include "../log.hpp"
#include "lodepng.h"
#include <fstream>
#include <string>

#define GL_RGB  0x1907
#define GL_RGBA 0x1908
#define GL_BGRA 0x80E1

namespace library
{
	Bitmap::Bitmap():
		buffer(nullptr), width(0), height(0), bits(0), format(0)
	{ }
	
	Bitmap::Bitmap(const std::string file, Bitmap::bitmap_type btype)
	{
		this->buffer = nullptr;
		load(file, btype);
	}
	Bitmap::Bitmap(int w, int h, int b): 
		width(w), height(h), bits(b), format(GL_BGRA)
	{
		buffer = new rgba8_t[width * height];
		if (buffer == nullptr)
		{
			throw std::string("Bitmap::Bitmap(int, int, int): Failed to allocate pixel buffer");
		}
	}
	Bitmap::~Bitmap()
	{
		delete[] buffer;
	}
	
	bool Bitmap::load(const std::string file, Bitmap::bitmap_type btype)
	{
		if (btype == BMP)
		{
			return loadBMP(file);
		}
		else if (btype == PNG)
		{
			return loadPNG(file);
		}
		throw std::string("Bitmap::load(): Unsupported format");
	}
	
	bool Bitmap::loadBMP(const std::string& file)
	{
		#pragma pack(push, 1)
		// packed: make sure bytes are packed tight, 1-byte memory alignment
		struct bmpheader_t
		{
			char magic[2];
			int filesize;
			short res1; 
			short res2;
			int pixeldata;
		};
		
		struct bmpdata_t
		{
			int headersize;
			int w;
			int h;
			short planes;
			short bits;
			// don't care about the rest
		};
		#pragma pack(pop)
		
		bmpheader_t bhead;
		bmpdata_t bdata;
		std::ifstream File (file.c_str(), std::ios::in | std::ios::binary);
		
		if (!File)
		{	// Log: could not open file
			logger << Log::ERR << "Could not open file: " << file << Log::ENDL;
			return false;
			//throw std::string("Bitmap::loadBMP(): Could not open file " + file);
		}
		
		File.read ( (char*) &bhead, sizeof(bmpheader_t) );
		File.read ( (char*) &bdata, sizeof(bmpdata_t) );
		
		this->width  = bdata.w;
		this->height = bdata.h;
		this->bits   = bdata.bits;
		this->format = GL_BGRA;
		this->tilesX = 1;
		this->tilesY = 1;
		
		int pixels = width * height;
		
		// delete any previous buffer
		delete[] buffer;
		// allocate 32-bits * w * h pixels
		buffer = new rgba8_t[pixels];
		
		// seek to pixeldata relative to beginning of file
		File.seekg(bhead.pixeldata);
		
		if (bdata.bits == 24)
		{
			// width of image on file in bytes
			int pad = (width * 3) & 3; // 4-byte padding
			if (pad) pad = 4 - pad;
			// width of image in memory in bytes
			int scanline = width * 3 + pad; // pitch
			
			char* scanbuffer = new char[scanline];
			char* buf = (char*)buffer;
			char* tmp;
			
			for (int y = 0; y < height; y++)
			{
				buf = (char*) buffer + (height-1 - y) * width * sizeof(rgba8_t);
				tmp = scanbuffer;
				// read entire scanline
				File.read(tmp, scanline);
				
				for (int x = 0; x < width; x++)
				{
					buf[0] = tmp[0];
					buf[1] = tmp[1];
					buf[2] = tmp[2];
					buf[3] = 255;
					buf += 4; tmp += 3;
				}
				
			}
			// cleanup
			delete scanbuffer;
		}
		else if (bdata.bits == 32)
		{
			// 32-bits (8 per channel)
			
			// width of image in bytes
			int scanline = width * sizeof(rgba8_t);
			char* scanpos = (char*) this->buffer + height * scanline;
			
			while (scanpos > (char*) this->buffer)
			{
				scanpos -= scanline;
				// read entire scanline
				File.read( scanpos, scanline );
			}
		}
		else
		{
			logger << Log::ERR << "Bitmap::loadBMP(): Invalid bits value: " << bdata.bits << Log::ENDL;
			//throw std::string("Bitmap::loadBMP(): Invalid bits value (" + file + ")");
		}
		File.close();
		return true;
	}
	
	bool Bitmap::loadPNG(const std::string& file)
	{
		std::vector<unsigned char> image, png;
		// load file from disk
		if (lodepng::load_file(png, file.c_str()) == false)
		{
			logger << Log::ERR << "Bitmap::loadPNG(): File not found: " << file << Log::ENDL;
			throw std::string("Bitmap::loadPNG(): File not found: " + file);
		}
		// decode it
		unsigned w, h;
		unsigned error = lodepng::decode(image, w, h, png);
		// throw if there are any errors
		if (error)
		{
			logger << Log::ERR << "Bitmap::loadPNG(): Error " << error << ": " << lodepng_error_text(error) << Log::ENDL;
			return false;
			//throw std::string("Bitmap::loadPNG(): Failed to decode " + file);
		}
		this->format = GL_RGBA;
		this->bits   = 32;
		this->tilesX = 1;
		this->tilesY = 1;
		
		this->width  = w;
		this->height = h;
		
		delete[] this->buffer;
		this->buffer = new rgba8_t[w * h];
		if (this->buffer == nullptr)
		{
			throw std::string("Bitmap::loadPNG(): Failed to allocate pixel buffer");
		}
		memcpy (this->buffer, image.data(), sizeof(rgba8_t) * w * h);
		
		return true;
	}
	
	void Bitmap::blit(Bitmap& dest, int srcX, int srcY, int width, int height, int dstX, int dstY) const
	{
		// copy from this at (x, y) with size (w, h) to destination at (dstX, dstY)
		for (int y = 0; y < height; y++)
		{
			rgba8_t* src = this->buffer + (y + srcY) * this->width + srcX;
			rgba8_t* dst = dest.buffer  + (y + dstY) * dest.getwidth() + dstX;
			
			memcpy (dst, src, width * sizeof(rgba8_t));
		}
	}
	
	// splits a bitmap in a 1d continous memory array
	// of tiles used by GL_TEXTURE_2D_ARRAY (GL 3.x)
	void Bitmap::parse2D(int tw, int th)
	{
		if (this->buffer == nullptr)
			throw std::string("Bitmap::parse2D(): Buffer was null");
		
		// nearest tilew/h multiple floor
		int maxx = int(this->width  / tw) * tw;
		int maxy = int(this->height / th) * th;
		
		// buffers
		rgba8_t* newBuffer = new rgba8_t[maxx * maxy];
		rgba8_t *p, *n = newBuffer;
		
		// for each tile
		int tx, ty;
		for (int y = th-1; y < maxy; y += th)
		for (int x = 0; x < maxx; x += tw)
		{
			//p = this->buffer + (y + th-1) * this->width + x;
			p = this->buffer + y * this->width + x;
			
			// blit internal tile
			for (ty = 0; ty < th; ty++)
			{
				for (tx = 0; tx < tw; tx++)
					*n++ = *p++;
				
				p -= this->width + tw;
			} // ty
			
		} // (x, y)
		
		// replace with new buffer
		delete[] this->buffer;
		this->buffer = newBuffer;
		// use members for texture_2d_array data
		this->tilesX = maxx / tw;
		this->tilesY = maxy / th;
		this->width  = tw;
		this->height = th;
	}
	void Bitmap::parse2D_invY(int tw, int th)
	{
		if (this->buffer == nullptr)
			throw std::string("Bitmap::parse2D(): Buffer was null");
		
		// nearest tilew/h multiple floor
		int maxx = int(this->width  / tw) * tw;
		int maxy = int(this->height / th) * th;
		
		// buffers
		rgba8_t* newBuffer = new rgba8_t[maxx * maxy];
		rgba8_t *p, *n = newBuffer;
		
		// for each tile
		int tx, ty;
		for (int y = 0; y < maxy; y += th)
		for (int x = 0; x < maxx; x += tw)
		{
			p = this->buffer + y * this->width + x;
			
			// blit internal tile
			for (ty = 0; ty < th; ty++)
			{
				for (tx = 0; tx < tw; tx++)
					*n++ = *p++;
				
				p += this->width - tw;
			} // ty
			
		} // (x, y)
		
		// replace with new buffer
		delete[] this->buffer;
		this->buffer = newBuffer;
		// use members for texture_2d_array data
		this->tilesX = maxx / tw;
		this->tilesY = maxy / th;
		this->width  = tw;
		this->height = th;
	}
	
	void Bitmap::replace(const unsigned int color, const unsigned int replacecolor)
	{	//Replaces one color witg another one in the picture.
		int n = width * height;
		for( int i = 0; i < n; i++ )
		{
			if( buffer[ i ] == color )
				buffer[ i ] = replacecolor;
		}
	}
	
	int Bitmap::getTilesX() const
	{
		return this->tilesX;
	}
	int Bitmap::getTilesY() const
	{
		return this->tilesY;
	}
	
	bool Bitmap::isValid() const
	{
		return (
			this->buffer != nullptr && 
			this->width > 0 && 
			this->height > 0
		);
	}
	
	Bitmap::rgba8_t Bitmap::makeColor(int r, int g, int b, int a)
	{
		rgba8_t tall;
		unsigned char *c = (unsigned char*) &tall;
		c[0] = b;
		c[1] = g;
		c[2] = r;
		c[3] = a;

		return tall;
	}
}
