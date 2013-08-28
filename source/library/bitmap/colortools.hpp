#ifndef COLORTOOLS_HPP
#define COLORTOOLS_HPP

#include "../math/toolbox.hpp"
#include "../math/vector.hpp"
#include <cstdint>

namespace library
{
	class vec4;
	typedef uint32_t color_rgba8_t;
	
	template <class C8, class C_8>
	color_rgba8_t BGRA8(C8 r, C8 g, C8 b, C_8 a)
	{
		return  ((color_rgba8_t)r      ) + 
				((color_rgba8_t)g <<  8) + 
				((color_rgba8_t)b << 16) + 
				((color_rgba8_t)a << 24);
	}
	
	template <class C8, class C_8>
	color_rgba8_t RGBA8(C8 r, C8 g, C8 b, C_8 a)
	{
		return  ((color_rgba8_t)b      ) + 
				((color_rgba8_t)g <<  8) + 
				((color_rgba8_t)r << 16) + 
				((color_rgba8_t)a << 24);
	}
	
	class cl_rgb
	{
		int r, g, b;
		
		static cl_rgb mixColor (cl_rgb& a, cl_rgb& b, float mixlevel);
		static void   addColorv(cl_rgb& a, cl_rgb& b, float level);
		static cl_rgb getGradientColor(float v, cl_rgb* array, int size);
		
	};
	
	vec4 colorToVector(color_rgba8_t color);
	color_rgba8_t vectorToColor(const vec4& vector);
}

#endif
