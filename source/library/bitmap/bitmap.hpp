#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <string>

namespace library
{
	class Bitmap
	{
	public:
		typedef unsigned int rgba8_t;
		
		enum bitmap_type
		{
			BMP,
			PNG
		};
		
		Bitmap();
		Bitmap(const std::string, bitmap_type);
		Bitmap(int w, int h, int bits);
		~Bitmap();
		
		bool load(const std::string file, bitmap_type);
		void replace(const rgba8_t color, const rgba8_t replacecolor);
		void blit(Bitmap& dest, int srcX, int srcY, int width, int height, int dstX, int dstY) const;
		void parse2D(int, int);
		void parse2D_invY(int, int);
		
		// getters
		inline rgba8_t* data() const
		{
			return this->buffer;
		}
		inline int getwidth() const
		{
			return this->width;
		}
		inline int getheight() const
		{
			return this->height;
		}
		inline int getbits() const
		{
			return this->bits;
		}
		inline int getFormat() const
		{
			return this->format;
		}
		
		int getTilesX() const;
		int getTilesY() const;
		
		// isValid: returns false if the bitmap is currently invalid
		bool isValid() const;
		
		static rgba8_t makeColor(int r, int g, int b, int a);
		static const rgba8_t NO_COLOR = 0x0;
		static const rgba8_t BLACK   = 0xFF000000;
		static const rgba8_t WHITE   = 0xFFFFFFFF;
		static const rgba8_t MAGENTA = 0xFFFF00FF;
		
	private:
		bool loadBMP(const std::string& file);
		bool loadPNG(const std::string& file);
		
		rgba8_t* buffer;
		int width, height, bits;
		int format, tilesX, tilesY;
	};
	
}

#endif
