#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <string>

namespace library
{
	class Bitmap
	{
	public:
		typedef unsigned int rgba8_t;
		
		// supported file-formats
		enum bitmap_type
		{
			BMP,
			PNG
		};
		
		// constructors
		Bitmap();
		Bitmap(const std::string, bitmap_type);
		Bitmap(int w, int h);
		Bitmap(int w, int h, rgba8_t color);
		~Bitmap();
		
		// assignment operator (creates new copy)
		Bitmap& operator= (const Bitmap& bmp);
		// copy constructor (creates new copy)
		Bitmap(const Bitmap& bmp)
		{
			this->operator=(bmp);
		}
		
		bool load(const std::string file, bitmap_type);
		
		// getters
		inline rgba8_t* data() const
		{
			return this->buffer;
		}
		inline int getWidth() const
		{
			return this->width;
		}
		inline int getHeight() const
		{
			return this->height;
		}
		inline int getFormat() const
		{
			return this->format;
		}
		// get pixel at (x, y)
		rgba8_t getPixel(int x, int y) const
		{
			return buffer[y * getWidth() + x];
		}
		// set pixel at (x, y)
		void setPixel(int x, int y, rgba8_t color)
		{
			buffer[y * getWidth() + x] = color;
		}
		
		int getTilesX() const;
		int getTilesY() const;
		
		// isValid: returns false if the bitmap is currently invalid
		bool isValid() const;
		
		// operations
		void clear(rgba8_t color);
		void replace(rgba8_t color, rgba8_t replacecolor);
		void blit(Bitmap& dest, int srcX, int srcY, int width, int height, int dstX, int dstY) const;
		void parse2D(int, int);
		void parse2D_invY(int, int);
		
		Bitmap rotate90() const;
		Bitmap flipX() const;
		Bitmap flipY() const;
		
	private:
		bool loadBMP(const std::string& file);
		bool loadPNG(const std::string& file);
		
		rgba8_t* buffer;
		int width, height;
		int format, tilesX, tilesY;
	};
	
}

#endif
