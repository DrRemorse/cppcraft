#include "voronoi.hpp"

#include "../noise/simplex1234.h"
#include "../math/vector.hpp"
#include <cmath>

namespace library
{
	Voronoi::vor_t Voronoi::vor_euclidian(vor_t x1, vor_t y1, vor_t x2, vor_t y2)
	{
		vor_t dx = x2-x1, dy = y2-y1;
		return sqrt(dx*dx + dy*dy);
	}
	
	Voronoi::vor_t Voronoi::vor_quadratic(vor_t x1, vor_t y1, vor_t x2, vor_t y2)
	{
		x1 -= x2; y1 -= y2;
		return x1*x1 + x1*y1 + y1*y1;
	}
	
	Voronoi::vor_t Voronoi::vor_linsquare(vor_t x1, vor_t y1, vor_t x2, vor_t y2)
	{
		x1 -= x2; y1 -= y2;
		return x1*x1 + y1*y1;
	}
	
	Voronoi::vor_t Voronoi::vor_minkowski(vor_t x1, vor_t y1, vor_t x2, vor_t y2)
	{
		const vor_t MINKOWSKI = 3.5;
		
		x1 -= x2; y1 -= y2;
		return std::pow(std::pow(fabs(x1), MINKOWSKI) + std::pow(fabs(y1), MINKOWSKI), 1.0 / MINKOWSKI);
	}
	
	Voronoi::vor_t Voronoi::vor_manhattan(vor_t x1, vor_t y1, vor_t x2, vor_t y2)
	{
		x1 -= x2; y1 -= y2;
		return fabs(x1) + fabs(y1);
	}
	
	Voronoi::vor_t Voronoi::vor_chebyshev(vor_t x1, vor_t y1, vor_t x2, vor_t y2)
	{
		x1 = fabs(x1 - x2);
		y1 = fabs(y1 - y2);
		
		if (x1 > y1) return x1;
		return y1;
	}
	
	Voronoi::vor_t Voronoi::voronoi(vor_t px, vor_t py, distance_func distFunc)
	{
		vor_t mindist = 99;
		int x = floor(px);
		int y = floor(py);
		
		for (int dx = x-1; dx <= x+1; dx++)
		for (int dy = y-1; dy <= y+1; dy++)
		{
			// create a voronoi point
			vor_t getx = dx + snoise2(px, py);
			vor_t gety = dy + snoise2(px + 431, py + 293);
			
			vor_t dist = distFunc( px, py, getx, gety );
			if (dist < mindist) mindist = dist;  // mew closest value
		}
		return mindist;
	}
	
	Voronoi::vor_t Voronoi::vordist(vor_t px, vor_t py, distance_func distFunc)
	{
		vor_t mindist = 99;
		int x = floor(px);
		int y = floor(py);
		//vor_t fx = px - x;
		//vor_t fy = py - y;
		
		vec3 res;
		
		for (int dx = x-1; dx <= x+1; dx++)
		for (int dy = y-1; dy <= y+1; dy++)
		{
			// create a voronoi point
			vor_t getx = dx + snoise2(px, py);
			vor_t gety = dy + snoise2(px + 431, py + 293);
			
			vor_t dist = distFunc( px, py, getx, gety );
			
			if (dist < mindist)
			{
				res.x = dist;
				res.y = px - getx;
				res.z = py - gety;
				
				mindist = dist;  // mew closest value
			}
		}
		return mindist;
	}
	
}

