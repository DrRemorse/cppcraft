#include "lighting.hpp"

#include "library/log.hpp"
#include "library/config.hpp"
#include "library/math/toolbox.hpp"
#include "blocks.hpp"
#include "flatlands.hpp"
#include "lighttable.hpp"
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
	
	const float LightingClass::DARKNESS = 256 * 0.96;
	static const float DARK_SHADOWS     = 256 * 0.62;
	const float LightingClass::SHADOWS  = 256 * 0.56;
	const float LightingClass::CORNERS  = 256 * 0.50;
	
	const float LightingClass::RAY_CRASH_CONSTANT = 16.0f;
	const float LightingClass::LIGHT_FULL_DAMAGE  = 12.0f;
	const float LightingClass::LIGHT_MEDI_DAMAGE  =  8.0f;
	
	void LightingClass::init()
	{
		// raycaster method
		this->ray_length = config.get("light.ray_length", 48);
		// clamp to some minimum value
		if (this->ray_length < 16) this->ray_length = 16;
		
		this->ray_count  = config.get("light.ray_count", 3);
		if (this->ray_count <  0) this->ray_count =  0;
		if (this->ray_count > 16) this->ray_count = 16;
		
		// crash factor based on distance and fixed crash constant
		this->ray_crash_factor = this->RAY_CRASH_CONSTANT / this->ray_length;
	}
	
	float lightComp(block_t id)
	{
		if (id < ALPHA_BARRIER) return Lighting.LIGHT_FULL_DAMAGE;
		if (isCross(id) || id == _LANTERN || id == _LADDER) return 0.0;
		return Lighting.LIGHT_MEDI_DAMAGE;
	}
	
	bool raySurvivalTest(block_t id, float& ray, float maxdmg, float& prediction, float distance_curve)
	{
		// add predicted damage
		if (prediction)
		{
			// halve damage for super transparent blocks
			if (id < BLEND_BARRIER)
			{
				// deal some dmg
				ray += prediction * distance_curve;
			}
			prediction = 0.0;
		}
		// exit if too much damage
		if (ray >= maxdmg)
		{
			ray = maxdmg;
			return true;
		}
		return false;
	}
	
	bool damageRay(block_t id, float& ray, float maxdmg, float& prediction, float distance_curve)
	{
		if (id < ALPHA_BARRIER)
		{
			ray += Lighting.LIGHT_FULL_DAMAGE; // * curve
			// check as little as possible wether or not this ray can survive!
			return raySurvivalTest(id, ray, maxdmg, prediction, distance_curve);
		}
		else if (isCross(id) || (id == _LANTERN) || (id == _LADDER) || (id == _VINES))
		{
			// do nothing
			return false;
		}
		else
		{
			ray += Lighting.LIGHT_MEDI_DAMAGE; // * curve
			// check as little as possible wether or not this ray can survive!
			return raySurvivalTest(id, ray, maxdmg, prediction, distance_curve);
		}
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
		float prediction = Lighting.ray_crash_factor * Lighting.ray_length;
		
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
				bxx >> 4, // Sector::BLOCKS_XZ, 
				byy >> 3, // Sector::BLOCKS_Y, 
				bzz >> 4  // Sector::BLOCKS_XZ
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
					if (damageRay(id, tmplight, maxlight, prediction, distance_curve))
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
		float prediction = Lighting.ray_crash_factor * Lighting.ray_length;
		
		int bxx, byy;
		int bzz = (int)position.z;
		
		int sectorz = bzz >> 4;
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
					if (damageRay(id, tmplight, maxlight, prediction, distance_curve))
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
	
	float LightingClass::lightRay1D(float tmplight, float maxlight, vec3 position)
	{
		int max_x = Sectors.getXZ() * Sector::BLOCKS_XZ;
		const int max_y = Sectors.getY() * Sector::BLOCKS_Y;
		
		// exit if we start out of bounds
		if (position.x < 0 || position.x >= max_x-0.5 || 
			position.y < 0 || 
			position.z < 0 || position.z >= max_x-0.5
		) return tmplight;
		
		block_t id;
		float inv_reach = 1.0 / Lighting.ray_length;
		float distance_curve = 1.0;
		float prediction = Lighting.ray_crash_factor * Lighting.ray_length;
		
		int sectorx = (int) position.x;
		int bxx = sectorx & (Sector::BLOCKS_XZ-1);
		sectorx >>= 4;
		int sectorz = (int) position.z;
		int bzz = sectorz & (Sector::BLOCKS_XZ-1);
		sectorz >>= 4;
		
		int skylevel = Flatlands(sectorx, sectorz)(bxx, bzz).skyLevel;
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
					if (damageRay(id, tmplight, maxlight, prediction, distance_curve))
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
	
	vertex_color_t LightingClass::lightCheck(LightList& list, Sector& sector, int bx, int by, int bz, int rayCount)
	{
		float tmplight = 0.0;
		int bxx = bx, byy = by, bzz = bz;
		
		// start from s, and work out possibly new position
		Sector* s2 = Spiders::spiderwrap(sector, bxx, byy, bzz);
		if (s2 == nullptr) goto theend;
		
	/* scope */;
	{
		vec2 angle = thesun.getAngle().xy();
		
		if (angle.y < 0)
		{
			angle.x = -angle.x;
			angle.y = -angle.y;
		}
		
		vec3 position = vec3(
			s2->x * Sector::BLOCKS_XZ + bxx, 
			s2->y * Sector::BLOCKS_Y  + byy,
			s2->z * Sector::BLOCKS_XZ + bzz
		);
		
		// pre-calculate darkness level
		// underground additional ray damage
		int darklevel = Flatlands.getGroundLevel(position.x, position.z) - 2;
		static const int darkramp  = 32;
		static const int underlevel = 64;
		
		float maxlight = SHADOWS;
		float light = 0.0;
		
		if (position.y < darklevel)
		{
			light = (darklevel - position.y) / darkramp;
			if (light > 1.0) light = 1.0;
			maxlight = SHADOWS * (1.0 - light) + light * DARK_SHADOWS;
			light = 0.0;
		}
		if (position.y < underlevel)
		{
			light = 1.0 - position.y / underlevel;
			maxlight = maxlight * (1.0 - light) + light * DARKNESS;
			light *= maxlight * 2.0;
		}
		
		#define sunray   lightRay2D(light, maxlight, position, angle.x, angle.y)
		#define halfray  lightRay2D(light, maxlight, position, half1.x, half1.y)
		
		if (rayCount <= 0)
		{
			// towards the sky
			tmplight = lightRay1D(light, maxlight, position);
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
				#define halfray2 lightRay2D(light, maxlight, position, half2.x, half2.y)
				
				tmplight = sunray * 0.6 + halfray * 0.2 + halfray2 * 0.2;
			}
			else
			{
				const int rays = 4;
				const int rounds = rayCount / 4;
				const float phi = PI * 2 / rays;
				
				for (int j = 1; j <= rounds; j++)
				{
					vec3 a = vec3(angle.x - j * 0.05, angle.y, 0);
					a.normalize();
					
					for (int i = 0; i < rays; i++)
					{
						tmplight += lightRay3D(light, maxlight, position, a);
						a = a.rotateOnAxis(thesun.getAngle(), phi);
					}
				}
				tmplight /= (float)rays * rounds;
				
				tmplight = sunray * 0.1 + tmplight * 0.9;
			}
		}
		
	} // end scope
	
	theend:
		
		// clamp to maximal darkness level
		if (tmplight > Lighting.DARKNESS) tmplight = Lighting.DARKNESS;
		
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
