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
	static const float LIGHT_MEDI_DAMAGE  = 14.0f;
	static const float LIGHT_FLUID_DAMAGE  = 8.0f;
	
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
	
	bool damageRay(block_t id, float& ray, float maxdmg, float distance_curve)
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
		else if (isFluid(id))
		{
			ray += LIGHT_FLUID_DAMAGE * distance_curve;
		}
		else
		{
			// remainder give light damage
			ray += LIGHT_MEDI_DAMAGE * distance_curve;
		}
		// exit everything if too much damage
		return ray >= maxdmg;
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
			if (s.contents == Sector::CONT_SAVEDATA)
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
	
	float LightingClass::fastRay3D(vec3 position, vec3 angle)
	{
		int max_x = Sectors.getXZ() * Sector::BLOCKS_XZ;
		const int max_y = Sectors.getY() * Sector::BLOCKS_Y;
		
		// exit if we start out of bounds
		if (position.x < 0.0f || position.x >= max_x-0.5f || 
			position.y < 0.0f || 
			position.z < 0.0f || position.z >= max_x-0.5f
		) return 1.0f;
		
		// avoid invalid angle
		if (angle.y < 0.0f) return 0.0;
		
		block_t id;
		int bxx, byy, bzz;
		int distance = Lighting.ray_length;
		float light = 1.0;
		
		while (distance > 0)
		{
			bxx = (int)position.x; // conversion from float to int
			byy = (int)position.y;
			bzz = (int)position.z;
			
			// world bounds check
			if (bxx < 0 || bxx >= max_x || bzz < 0 || bzz >= max_x || byy >= max_y)
				return 1.0f;
			
			// set current sector
			Sector& s = Sectors(
				bxx >> Sector::BLOCKS_XZ_SH,
				byy >> Sector::BLOCKS_Y_SH,
				bzz >> Sector::BLOCKS_XZ_SH
			);
			
			// calculate lighting cost
			if (s.contents == Sector::CONT_SAVEDATA)
			{
				bxx &= (Sector::BLOCKS_XZ-1);
				byy &= (Sector::BLOCKS_Y-1);
				bzz &= (Sector::BLOCKS_XZ-1);
				
				// get block-id from location
				id = s(bxx, byy, bzz).getID();
				// avoid running costly function on _AIR
				if (id) // _AIR is always 0
				{
					if (id < ALPHA_BARRIER)
					{
						light -= 0.3f;
						if (light <= 0.0f) return 0.0f;
					}
					else if (isCross(id))
					{
						// do nothing
					}
					else
					{
						light -= 0.1f;
						if (light <= 0.0f) return 0.0f;
					}
				}	
				
				// single ray step
				position += angle;
				distance--;
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
				distance -= bxx;
				// big ray step
				position += angle * bxx;
			}
			
		} // while loop
		
		return light;
	}
	
	float LightingClass::lightRay2D(vec3 position, float stepx, float stepy)
	{
		int max_x = Sectors.getXZ() * Sector::BLOCKS_XZ;
		const int max_y = Sectors.getY() * Sector::BLOCKS_Y;
		
		// exit if we start out of bounds
		if (position.x < 0 || position.x >= max_x-0.5 || 
			position.y < 0 || 
			position.z < 0 || position.z >= max_x-0.5
		) return 1.0f;
		
		// avoid invalid angle
		if (stepy < 0) return 0.0f;
		
		block_t id;
		float tmplight = 0.0f;
		float maxlight = 1.0f;
		float lightFactor = 1.0f / Lighting.ray_length;
		
		int bxx, byy;
		int bzz = (int)position.z;
		
		int sectorz = bzz >> Sector::BLOCKS_XZ_SH;
		bzz &= (Sector::BLOCKS_XZ-1);
		
		int distance = Lighting.ray_length;
		while (distance > 0)
		{
			bxx = (int)position.x; // conversion from float to int
			byy = (int)position.y;
			
			// world bounds check
			if (bxx < 0 || bxx >= max_x || byy >= max_y)
			{
				tmplight += lightFactor * distance;
				if (tmplight >= maxlight) return maxlight;
				return tmplight;
			}
			
			// set current sector
			Sector& s = Sectors(bxx >> 4, byy >> 3, sectorz);
			
			// calculate lighting cost
			if (s.contents == Sector::CONT_SAVEDATA)
			{
				bxx &= (Sector::BLOCKS_XZ-1);
				byy &= (Sector::BLOCKS_Y-1);
				
				// get block-id from location
				id = s(bxx, byy, bzz).getID();
				// avoid running costly function on _AIR
				if (id)
				{
					if (id < ALPHA_BARRIER)
					{
						maxlight *= 0.95;
					}
					else if (isCross(id) || (id == _LANTERN) || (id == _LADDER) || (id == _VINES))
					{
						// we ignore all these blocks
					}
					else if (isFluid(id))
					{
						maxlight *= 0.97;
					}
					else
					{
						maxlight *= 0.96;
					}
				}
				else // _AIR == 0
				{
					// if this position is exposed to atmosphere
					if (position.y > flatlands(s.getX(), sectorz)(bxx, bzz).groundLevel)
					{
						tmplight += lightFactor;
						if (tmplight >= maxlight) return maxlight;
					}
				}
			}
			else
			{
				bxx &= (Sector::BLOCKS_XZ-1);
				
				// if this position is exposed to atmosphere
				if (position.y > flatlands(s.getX(), sectorz)(bxx, bzz).groundLevel)
				{
					tmplight += lightFactor;
					if (tmplight >= maxlight) return maxlight;
				}
			}
			// single ray step
			position.x += stepx;
			position.y += stepy;
			distance--;
			
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
			if (s.contents == Sector::CONT_SAVEDATA)
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
	
	inline bool LightingClass::light1D(int x, int y, int z)
	{
		if (y >= 64)
			return y <= flatlands.getSkyLevel(x, z);
		return y < flatlands.getGroundLevel(x, z);
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
		int rad;
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
						//goto endLight;
						if (lightSeeker(px, py, pz, px+x, pz+rad)) goto endLight;
					if (light1D(px+x, py, pz-rad) == false)
						//goto endLight;
						if (lightSeeker(px, py, pz, px+x, pz-rad)) goto endLight;
				}
			}
			for (z = -rad; z <= rad; z++)
			{
				if (radSquared + z*z <= radiusSquared)
				{
					if (light1D(px+rad, py, pz + z) == false)
						//goto endLight;
						if (lightSeeker(px, py, pz, px+rad, pz+z)) goto endLight;
					if (light1D(px-rad, py, pz + z) == false)
						//goto endLight;
						if (lightSeeker(px, py, pz, px-rad, pz+z)) goto endLight;
				}
			}
		}
		return 1.0f;
		endLight:
		return std::min(1.0f, sqrtf(x*x + z*z) / maxRadius);
	}
	
	vertex_color_t LightingClass::lightCheck(LightList& list, Sector& sector, int bx, int by, int bz, int norm_int, int rayCount)
	{
		vec3 position = vec3(
			sector.getX() * Sector::BLOCKS_XZ + bx, 
			sector.getY() * Sector::BLOCKS_Y  + by, 
			sector.getZ() * Sector::BLOCKS_XZ + bz
		);
		
		float tmplight = 0.0f;
		
		vec2 angle = thesun.getAngle().xy();
		if (angle.y < 0.0f) goto straightToTorchlight;
		
		#define sunray   lightRay2D(position, angle.x, angle.y)
		#define halfray  lightRay2D(position, half1.x, half1.y)
		#define skyray   lightRay1D(tmplight, SHADOWS, position)
		
		// skylevel searching (light seeking)
		// used as base shadows for all configurations
		// if the first block is black we can try to find distance to closest atmospheric light
		//if (position.y < 64)
		/*if (light1D(position.x, position.y, position.z))
		{
			float v = 0.5f + ((position.y < 64) ? 0.0f : 0.5f);
			int rad = this->seek_radius * v;
			float dist = lightSeek(rad, position.x, position.y, position.z);
			tmplight = DARKNESS * dist;
		}*/
		
		if (true)  //tmplight < SHADOWS)
		{
			if (rayCount <= 0)
			{
				// do nothing!
			}
			else if (rayCount == 1)
			{
				// towards sun
				tmplight = sunray * 255.0f;
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
					#define halfray2 lightRay2D(position, half2.x, half2.y)
					
					tmplight = sunray * 0.6 + halfray * 0.2 + halfray2 * 0.2;
					tmplight *= 255.0f;
				}
				else
				{
					/*
					float light = fastRay3D(position, normal);
					
					static const int ROTATIONS = 8;
					static const float phi = PI / ROTATIONS;
					static const int ROUNDS = 1;
					
					for (int r = 0; r < ROUNDS; r++)
					{
						vec3 a = rotator + thesun.getAngle() * r;
						a.normalize();
						vec3 axis = normal + thesun.getAngle() * r;
						axis.normalize();
						
						for (int i = -ROTATIONS; i < ROTATIONS; i++)
						{
							light += fastRay3D(position, a);
							a = a.rotateOnAxis(axis, phi);
						}
					}
					//
					light /= ROTATIONS * (ROUNDS + 1) + 1;
					*/
					//tmplight = light * 255.0f;
					// pepper some slight sunray into it all
					tmplight = sunray * 255.0f;
					//tmplight = (sunray + light) * 0.5f * 255.0f;
				}
			}
			if (tmplight > 255.0)
				tmplight = 255.0;
			else if (tmplight < 0.0)
				tmplight = 0.0;
			
		} // raycasting when light < SHADOWS
		
	straightToTorchlight:
		if (list.lights.size() == 0)
		{
			// shadows only, since there are no lights
			return vertex_color_t(tmplight) | (255 << 16);
		}
		else
		{
			// apply emissive lights
			return torchlight.torchlight(list, tmplight, sector, bx, by, bz);
		}
	}
}
