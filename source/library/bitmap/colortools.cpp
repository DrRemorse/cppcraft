#include "colortools.hpp"

namespace library
{
	cl_rgb cl_rgb::mixColor(cl_rgb& a, cl_rgb& b, float mixlevel)
	{
		if (mixlevel == 0) return a;
		
		cl_rgb c;
		c.r = (int)( (float)a.r * (1.0 - mixlevel) + (float)b.r * mixlevel );
		if (c.r > 255) c.r = 255;
		c.g = (int)( (float)a.g * (1.0 - mixlevel) + (float)b.g * mixlevel );
		if (c.g > 255) c.g = 255;
		c.b = (int)( (float)a.b * (1.0 - mixlevel) + (float)b.b * mixlevel );
		if (c.b > 255) c.b = 255;
		return c;
	}
	
	void cl_rgb::addColorv(cl_rgb& a, cl_rgb& b, float level)
	{
		if (level == 0.0) return;
		
		a.r += (int)( (float)b.r * level );
		if (a.r > 255) a.r = 255;
		a.g += (int)( (float)b.g * level );
		if (a.g > 255) a.g = 255;
		a.b += (int)( (float)b.b * level );
		if (a.b > 255) a.b = 255;
	}
	
	cl_rgb cl_rgb::getGradientColor(float v, cl_rgb* array, int size)
	{
		int    vint = (int)v, vnxt;
		float  vfrac = v - vint;
		
		// find color gradient values
		if (vfrac < 0.5)
			vnxt = vint - 1;
		else
			vnxt = vint + 1;
		
		// clamp
		if (vnxt < 0) vnxt = 0;
		if (vnxt >= size) vnxt = size-1;
		
		// get gradient array colors
		cl_rgb& cl1 = array[vint];
		cl_rgb& cl2 = array[vnxt];
		
		// convert fractional to interpolator
		if (vfrac < 0.5) vfrac = (0.5 - vfrac) / 0.5;
		else vfrac = (vfrac - 0.5) / 0.5;
		// S-curve the interpolator
		vfrac = toolbox::quintic(vfrac);
		
		return mixColor( cl1, cl2, vfrac * 0.5 );
	}
	
	vec4 colorToVector(color_rgba8_t color)
	{
		return vec4
		(
			 (color & 255) / 255.0,
			((color >>  8) & 255) / 255.0,
			((color >> 16) & 255) / 255.0,
			 (color >> 24) / 255.0
		);
	}
	
	color_rgba8_t vectorToColor(const vec4& vector)
	{
		int r = int(vector.x * 255);
		int g = int(vector.y * 255) << 8;
		int b = int(vector.y * 255) << 16;
		int a = int(vector.y * 255) << 24;
		return BGRA8(r, g, b, a);
	}
	
}
