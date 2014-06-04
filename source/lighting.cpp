#include "lighting.hpp"

#include <library/log.hpp>
#include <library/config.hpp>
#include <library/math/toolbox.hpp>
#include "blocks.hpp"
#include "flatlands.hpp"
#include "lighttable.hpp"
#include "renderconst.hpp"
#include "sectors.hpp"
#include "spiders.hpp"
#include "sun.hpp"
#include "torchlight.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	const float PI = atan(1.0) * 4;
	LightingClass Lighting;
	
	const float LightingClass::DARKNESS = 255;
	static const float DARK_SHADOWS     = 255 * 0.78;
	const float LightingClass::SHADOWS  = 255 * 0.65;
	const float LightingClass::AMB_OCC  = 255 * 0.48;
	const float LightingClass::CORNERS  = 255 * 0.55;
	
	static const float RAY_CRASH_CONSTANT = 64.0f;
	static const float LIGHT_FULL_DAMAGE  = 24.0f;
	static const float LIGHT_MEDI_DAMAGE  = 10.0f;
	
	void LightingClass::init()
	{
		// raycaster method
		this->ray_length = config.get("light.ray_length", 48);
		// clamp to some minimum value
		if (this->ray_length < 32) this->ray_length = 32;
		
		this->ray_count  = config.get("light.ray_count", 1);
		if (this->ray_count < 0) this->ray_count = 0;
		
		this->seek_radius = config.get("light.seek_radius", 4);
		if (this->seek_radius < 1) this->seek_radius = 1;
	}
	
	bool damageRay(block_t id, float& ray, float& maxdmg, float distance_curve)
	{
		if (id < ALPHA_BARRIER)
		{
			ray += LIGHT_FULL_DAMAGE * distance_curve;
		}
		else if (isCross(id) || (id == _LANTERN) || (id == _LADDER) || (id == _VINES))
		{
			// we ignore all these blocks
			return false;
		}
		else
		{
			// remainder give light damage
			ray += LIGHT_MEDI_DAMAGE * distance_curve;
		}
		// exit everything if too much damage
		if (ray >= maxdmg) return true;
		return false;
	}
	
	inline int rayStep(float angle, int vv, int maxv)
	{
		const double EPSILON = 0.0001;
		
		if (angle > EPSILON)
		{
			// find remaining
			vv = maxv - (vv & maxv);
			// calculate steps needed:
			return  vv / angle + 1;
		}
		else if (angle < -EPSILON)
		{
			// find remaining
			vv = (vv & maxv);
			// calculate steps needed:
			return -vv / angle + 1;
		}
		else return 10000;
	}
	
	float LightingClass::lightRay3D(float tmplight, float maxlight, vec3 position, vec3 angle)
	{
		int max_x = Sectors.getXZ() * Sector::BLOCKS_XZ;
		const int max_y = Sectors.getY() * Sector::BLOCKS_Y;
		
		// exit if we start out of bounds
		if (position.x < 0 || position.x >= max_x-0.5 || 
			position.y < 0 || 
			position.z < 0 || position.z >= max_x-0.5
		) return 0.0;
		
		// avoid invalid angle
		if (angle.y < 0) return maxlight;
		
		block_t id;
		float inv_reach = 1.0 / Lighting.ray_length;
		float distance_curve = 1.0;
		
		int bxx, byy, bzz;
		
		while (distance_curve >= inv_reach)
		{
			bxx = (int)position.x; // conversion from float to int
			byy = (int)position.y;
			bzz = (int)position.z;
			
			// world bounds check
			if (bxx < 0 || bxx >= max_x || bzz < 0 || bzz >= max_x || byy >= max_y)
				return tmplight;
			
			// set current sector
			Sector& s = Sectors(
				bxx >> Sector::BLOCKS_XZ_SH,
				byy >> Sector::BLOCKS_Y_SH,
				bzz >> Sector::BLOCKS_XZ_SH
			);
			
			// calculate lighting cost
			if (s.contents > Sector::CONT_UNKNOWN)
			{
				bxx &= (Sector::BLOCKS_XZ-1);
				byy &= (Sector::BLOCKS_Y-1);
				bzz &= (Sector::BLOCKS_XZ-1);
				
				// get block-id from location
				id = s(bxx, byy, bzz).getID();
				// avoid running costly function on _AIR
				if (id) // _AIR is always 0
				{
					// ray damage function
					if (damageRay(id, tmplight, maxlight, distance_curve))
					{
						// the ray has died, exit
						return tmplight;
					}
				}	
				
				// single ray step
				position += angle;
				distance_curve -= inv_reach;
			}
			else
			{
				// move ray immediately to the next sector
				bxx = rayStep(angle.x, bxx, (Sector::BLOCKS_XZ-1));
				byy = rayStep(angle.y, byy, (Sector::BLOCKS_Y-1));
				bzz = rayStep(angle.z, bzz, (Sector::BLOCKS_XZ-1));
				
				// choose the shortest distance
				if (bxx > byy)
				{	// NOT BXX
					
					if (bzz > byy)
					{	// NOT BZZ, BXX
						bxx = byy;
					}
					else
					{
						// NOT BYY, BXX
						bxx = bzz;
					}
					
				}	// NOT BYY
				else if (bzz < bxx)
				{
					bxx = bzz;
				}
				
				// move stepx steps
				distance_curve -= inv_reach * bxx;
				// big ray step
				position += angle * bxx;
			}
			
		} // while loop
		
		return tmplight;
	}
	
	float LightingClass::lightRay2D(float tmplight, float maxlight, vec3 position, float stepx, float stepy)
	{
		int max_x = Sectors.getXZ() * Sector::BLOCKS_XZ;
		const int max_y = Sectors.getY() * Sector::BLOCKS_Y;
		
		// exit if we start out of bounds
		if (position.x < 0 || position.x >= max_x-0.5 || 
			position.y < 0 || 
			position.z < 0 || position.z >= max_x-0.5
		) return 0.0;
		
		// avoid invalid angle
		if (stepy < 0) return maxlight;
		
		block_t id;
		float inv_reach = 1.0 / Lighting.ray_length;
		float distance_curve = 1.0;
		
		int bxx, byy;
		int bzz = (int)position.z;
		
		int sectorz = bzz >> Sector::BLOCKS_XZ_SH;
		bzz &= (Sector::BLOCKS_XZ-1);
		
		while (distance_curve >= inv_reach)
		{
			bxx = (int)position.x; // conversion from float to int
			byy = (int)position.y;
			
			// world bounds check
			if (bxx < 0 || bxx >= max_x || byy >= max_y)
				return tmplight;
			
			// set current sector
			Sector& s = Sectors(bxx >> 4, byy >> 3, sectorz);
			
			// calculate lighting cost
			if (s.contents > Sector::CONT_UNKNOWN)
			{
				bxx &= (Sector::BLOCKS_XZ-1);
				byy &= (Sector::BLOCKS_Y-1);
				
				// get block-id from location
				id = s(bxx, byy, bzz).getID();
				// avoid running costly function on _AIR
				if (id) // _AIR is always 0
				{
					// ray damage function
					if (damageRay(id, tmplight, maxlight, distance_curve))
					{
						// the ray has died, exit
						return tmplight;
					}
				}	
				
				// single ray step
				position.x += stepx;
				position.y += stepy;
				distance_curve -= inv_reach;
			}
			else
			{
				// move ray immediately to the next sector
				bxx = rayStep(stepx, bxx, (Sector::BLOCKS_XZ-1));
				byy = rayStep(stepy, byy, (Sector::BLOCKS_Y-1));
				
				// choose the shortest distance
				if (bxx > byy) bxx = byy;
				
				if (bxx == 1)
				{
					distance_curve -= inv_reach;
					// big ray step
					position.x += stepx;
					position.y += stepy;
				}
				else
				{
					// move stepx steps
					distance_curve -= inv_reach * bxx;
					// big ray step
					position.x += stepx * bxx;
					position.y += stepy * bxx;
				}
			}
			
		} // while loop
		
		return tmplight;
	}
	
	float LightingClass::lightRay1D(float tmplight, float maxlight, vec3 const& position)
	{
		int max_x = Sectors.getXZ() * Sector::BLOCKS_XZ;
		const int max_y = Sectors.getY() * Sector::BLOCKS_Y;
		
		// exit if we start out of bounds
		if (position.x < 0 || position.x >= max_x-0.5 || 
			position.y < 0 || 
			position.z < 0 || position.z >= max_x-0.5
		) return tmplight;
		
		int sectorx = (int) position.x;
		int bxx = sectorx & (Sector::BLOCKS_XZ-1);
		sectorx >>= Sector::BLOCKS_XZ_SH;
		int sectorz = (int) position.z;
		int bzz = sectorz & (Sector::BLOCKS_XZ-1);
		sectorz >>= Sector::BLOCKS_XZ_SH;
		
		int skylevel = flatlands(sectorx, sectorz)(bxx, bzz).skyLevel;
		
		block_t id;
		float inv_reach = 1.0 / Lighting.ray_length;
		float distance_curve = 1.0;
		
		int posy = (int) position.y;
		int byy;
		
		while (distance_curve >= inv_reach)
		{
			// world bounds check
			if (posy >= max_y || posy >= skylevel) return tmplight;
			
			// set current sector
			Sector& s = Sectors(sectorx, posy >> 3, sectorz);
			
			// calculate lighting cost
			if (s.contents > Sector::CONT_UNKNOWN)
			{
				byy = posy & (Sector::BLOCKS_Y-1);
				
				// get block-id from location
				id = s(bxx, byy, bzz).getID();
				// avoid running costly function on _AIR
				if (id) // _AIR is always 0
				{
					// ray damage function
					if (damageRay(id, tmplight, maxlight, distance_curve))
					{
						// the ray has died, exit
						return tmplight;
					}
				}	
				
				// single ray step
				posy += 1;
				distance_curve -= inv_reach;
			}
			else
			{
				// move ray immediately to the next sector
				byy = Sector::BLOCKS_Y - (posy & (Sector::BLOCKS_Y-1));
				
				// move stepx steps
				distance_curve -= inv_reach * byy;
				// big ray step
				posy += byy;
			}
			
		} // while loop
		
		return tmplight;
	}
	
	bool LightingClass::light1D(int x, int y, int z)
	{
		int sectorx = x >> Sector::BLOCKS_XZ_SH;
		int bxx = x & (Sector::BLOCKS_XZ-1);
		
		int sectorz = z >> Sector::BLOCKS_XZ_SH;
		int bzz = z & (Sector::BLOCKS_XZ-1);
		
		if (y >= 64)
			return (y < flatlands(sectorx, sectorz)(bxx, bzz).skyLevel);
		return (y < flatlands(sectorx, sectorz)(bxx, bzz).groundLevel);
	}
	
	inline bool isSeekableID(block_t id)
	{
		return id == _AIR || isCross(id) || id == _LADDER || id == _VINES || isFluid(id);
	}
	inline bool isSeekable(int x, int y, int z)
	{
		return isSeekableID(Spiders::getBlock(x, y, z).getID());
	}
	
	bool lightSeeker(int x1, int y1, int z1, int x2, int z2)
	{
		// select next position
		// ALSO: avoid checking FIRST position
		if (x1 < x2) x1 += 1;
		else if (x1 > x2) x1 -= 1;
		
		// We also can't avoid advancing both positions
		// because some blocks self-occlude into (1x, 1z) diagonally
		if (z1 < z2) z1 += 1;
		else if (z1 > z2) z1 -= 1;
		
		// exit condition
		if (x1 == x2 && z1 == z2) return true;
		
		// validate position
		if (isSeekable(x1, y1, z1) == false)
		{
			return false;
		}
		
		bool select_x = true;
		bool moved;
		int x = x1;
		int z = z1;
		
		while (true)
		{
			// select next position
			if (select_x)
			{
				if (x1 < x2) x = x1 + 1;
				else if (x1 > x2) x = x1 - 1;
				else x = x1;
			}
			else
			{
				if (z1 < z2) z = z1 + 1;
				else if (z1 > z2) z = z1 - 1;
				else z = z1;
			}
			
			// exit condition
			if (x1 == x2 && z1 == z2) return true;
			moved = false;
			
			// validate position
			if (isSeekable(x, y1, z))
			{
				x1 = x;
				z1 = z;
				moved = true;
			}
			if (isSeekable(x1, y1+1, z1))
			{
				// move up instead of toward
				y1 += 1;
				moved = true;
			}
			
			if (moved == false)
			{
				// bad
				return false;
			}
			
			select_x = !select_x;
		}
	} // lightSeeker
	
	float LightingClass::lightSeek(int maxRadius, int px, int py, int pz)
	{
		int rad = 0;
		int x = 0, z = 0;
		int radiusSquared = maxRadius * maxRadius;
		int radSquared;
		
		for (rad = 1; rad <= maxRadius; rad++)
		{
			radSquared = rad*rad;
			
			for (x = -rad; x <= rad; x++)
			{
				if (x*x + radSquared <= radiusSquared)
				{
					if (light1D(px+x, py, pz+rad) == false)
						goto endLight;
						//if (lightSeeker(px, py, pz, px+x, pz+rad)) goto endLight;
					if (light1D(px+x, py, pz-rad) == false)
						goto endLight;
						//if (lightSeeker(px, py, pz, px+x, pz-rad)) goto endLight;
				}
			}
			for (z = 1-rad; z < rad; z++)
			{
				if (radSquared + z*z <= radiusSquared)
				{
					if (light1D(px+rad, py, pz + z) == false)
						goto endLight;
						//if (lightSeeker(px, py, pz, px+rad, pz+z)) goto endLight;
					if (light1D(px-rad, py, pz + z) == false)
						goto endLight;
						//if (lightSeeker(px, py, pz, px-rad, pz+z)) goto endLight;
				}
			}
		}
		return 1.0;
		endLight:
		return sqrtf(x*x + z*z) / (float)maxRadius;
	}
	
	vertex_color_t LightingClass::lightCheck(LightList& list, Sector& sector, int bx, int by, int bz, int rayCount)
	{
		float tmplight = 0.0;
		
		vec3 position = vec3(
			sector.x * Sector::BLOCKS_XZ + bx, 
			sector.y * Sector::BLOCKS_Y  + by, 
			sector.z * Sector::BLOCKS_XZ + bz
		);
		
		vec2 angle = thesun.getAngle().xy();
		if (angle.y < 0.0) tmplight = SHADOWS;
		
		#define sunray   lightRay2D(tmplight, SHADOWS, position, angle.x, angle.y)
		#define halfray  lightRay2D(tmplight, SHADOWS, position, half1.x, half1.y)
		#define skyray   lightRay1D(tmplight, SHADOWS, position)
		
		// skylevel searching (light seeking)
		// used as base shadows for all configurations
		// if the first block is black we can try to find distance to closest atmospheric light
		if (light1D(position.x, position.y, position.z))
		{
			float dist = lightSeek(this->seek_radius, position.x, position.y, position.z);
			tmplight += DARKNESS * dist;
			
			if (tmplight > DARKNESS) tmplight = DARKNESS;
		}
		
		if (tmplight < SHADOWS)
		{
			if (rayCount <= 0)
			{
				// do nothing!
			}
			else if (rayCount == 1)
			{
				// towards sun
				tmplight = sunray;
			}
			else if (rayCount == 2)
			{
				const vec3& half1 = thesun.getHalfAngle();
				// towards sun & halfray
				tmplight = sunray * 0.8 + halfray * 0.2;
			}
			else
			{
				if (rayCount == 3)
				{
					const vec3& half1 = thesun.getHalfAngle();
					const vec3& half2 = thesun.getHalf2Angle();
					
					// additional halfrays
					#define halfray2 lightRay2D(tmplight, SHADOWS, position, half2.x, half2.y)
					
					tmplight = sunray * 0.6 + halfray * 0.2 + halfray2 * 0.2;
				}
				else
				{
					static const int ROTATIONS = 4;
					static const float phi = PI * 2 / ROTATIONS;
					const int rounds = rayCount / 4;
					
					float light = SHADOWS;
					for (int j = 0; j < rounds; j++)
					{
						vec3 a(angle.x - j * 0.05, angle.y, 0.15);
						a.normalize();
						
						for (int i = 0; i < ROTATIONS; i++)
						{
							light = std::min(light, lightRay3D(tmplight, SHADOWS, position, a));
							a = a.rotateOnAxis(thesun.getAngle(), phi);
						}
					}
					//light /= (float)rays * rounds;
					// pepper some slight sunray into it all
					tmplight = sunray * 0.1 + light * 0.9;
				}
			}
			if (tmplight > SHADOWS)
				tmplight = SHADOWS;
			
		} // raycasting when light < SHADOWS
		
		if (list.lights.size() == 0)
		{
			// shadows only, since there are no lights
			return vertex_color_t(255 - tmplight) + (255 << 16);
		}
		else
		{
			// apply emissive lights
			return torchlight.torchlight(list, tmplight, sector, bx, by, bz);
		}
	}
}
