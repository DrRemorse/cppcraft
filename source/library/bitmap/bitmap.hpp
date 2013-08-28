#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <string>

namespace library
{
	class Bitmap
	{
	public:
		typedef unsigned int color_rgba8_t;
		
		Bitmap();
		Bitmap(const std::string);
		Bitmap(int w, int h, int bits);
		~Bitmap();
		
		bool load(const std::string file);
		void replace(const color_rgba8_t color, const color_rgba8_t replacecolor);
		void blit(Bitmap& dest, int srcX, int srcY, int width, int height, int dstX, int dstY) const;
		void parse2D(int, int);
		
		// getters
		color_rgba8_t* data() const;
		int getwidth()  const;
		int getheight() const;
		int getbits()   const;
		
		int getTilesX() const;
		int getTilesY() const;
		
		// isValid: returns false if the bitmap is currently invalid
		bool isValid() const;
		
		static color_rgba8_t makeColor(int r, int g, int b, int a);
		static const color_rgba8_t NO_COLOR = 0x0;
		static const color_rgba8_t BLACK   = 0xFF000000;
		static const color_rgba8_t WHITE   = 0xFFFFFFFF;
		static const color_rgba8_t MAGENTA = 0xFFFF00FF;
		
	private:
		color_rgba8_t* buffer;
		int width, height, bits;
		int tilesX, tilesY;
	};
	
}

#endif
