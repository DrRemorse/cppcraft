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
		
		// getters
		rgba8_t* data() const;
		int getwidth()  const;
		int getheight() const;
		int getbits()   const;
		
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
		
		rgba8_t* buffer;
		int width, height, bits;
		int tilesX, tilesY;
	};
	
}

#endif
