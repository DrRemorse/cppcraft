#include "lighting.hpp"

#include "library/log.hpp"
#include "library/config.hpp"
#include "blocks.hpp"
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
	
	const float LightingClass::MAX_ADDITIONAL_LIGHT = 0.95f;
	
	const float LightingClass::DARKNESS = 256 * 0.90;
	const float LightingClass::SHADOWS  = 256 * 0.68;
	const float LightingClass::CORNERS  = 256 * 0.45;
	
	const float LightingClass::RAY_CRASH_CONSTANT = 16.0f;
	const float LightingClass::LIGHT_FULL_DAMAGE  = 12.0f;
	const float LightingClass::LIGHT_MEDI_DAMAGE  =  8.0f;
	
	void LightingClass::init()
	{
		this->ray_length = 48;
		this->ray_count  = 2;
		
		// raycaster method
		this->ray_length = config.get("light.ray_length", this->ray_length);
		// clamp to some minimum value
		if (this->ray_length < 16) this->ray_length = 16;
		
		this->ray_count  = config.get("light.ray_count", this->ray_count);
		if (this->ray_count < 1) this->ray_count = 1;
		if (this->ray_count > 6) this->ray_count = 6;
		
		// crash factor based on distance and fixed crash constant
		this->ray_crash_factor = this->RAY_CRASH_CONSTANT / this->ray_length;
	}
	
	float lightComp(block_t id)
	{
		if (id < ALPHA_BARRIER) return Lighting.LIGHT_FULL_DAMAGE;
		if (isCross(id) || id == _LANTERN || id == _LADDER) return 0.0;
		return Lighting.LIGHT_MEDI_DAMAGE;
	}
	
	bool LightingClass::raySurvivalTest(block_t id, float& ray, float maxdmg, float& prediction, float distance_curve)
	{
		// add predicted damage
		if (prediction)
		{
			// halve damage for super transparent blocks
			if (id < BLEND_BARRIER)
			{
				// deal some dmg
				ray += prediction * sqrtf(distance_curve);
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
	
	bool LightingClass::damageRay(block_t id, float& ray, float maxdmg, float& prediction, float distance_curve)
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
	
	int rayStep(float angle, int vv, int maxv)
	{
		if (angle > 0.0001)
		{
			// find remaining
			vv = maxv - (vv & maxv);
			// calculate steps needed:
			return  vv / angle + 1;
		}
		else if (angle < -0.0001)
		{
			// find remaining
			vv = (vv & maxv);
			// calculate steps needed:
			return -vv / angle + 1;
		}
		else return 10000;
	}
	
	float LightingClass::lightRay3D(vec3 position, vec3 angle)
	{
		int max_x = Sectors.getXZ() * Sector::BLOCKS_XZ;
		const int max_y = Sectors.getY() * Sector::BLOCKS_Y;
		
		// exit if we start out of bounds
		if (position.x < 0 || position.x >= max_x-0.5 || 
			position.y < 0 || 
			position.z < 0 || position.z >= max_x-0.5
		) return 0.0;
		
		float maxlight = Lighting.SHADOWS;
		float tmplight = 0;
		
		// underground additional ray damage
		if (position.y < 64)
		{
			tmplight = 1.0 - position.y / 64;
			maxlight += tmplight * (Lighting.DARKNESS - Lighting.SHADOWS);
			tmplight *= maxlight;
		}
		
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
						// the ray has died, exit while
						break;
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
				if (distance_curve <= inv_reach) return tmplight;
				
				// big ray step
				position += angle * bxx;
			}
			
		} // while loop
		
		return tmplight;
	}
	
	float LightingClass::lightRay1D(vec3 position)
	{
		int max_x = Sectors.getXZ() * Sector::BLOCKS_XZ;
		const int max_y = Sectors.getY() * Sector::BLOCKS_Y;
		
		// exit if we start out of bounds
		if (position.x < 0 || position.x >= max_x-0.5 || 
			position.y < 0 || 
			position.z < 0 || position.z >= max_x-0.5
		) return 0.0;
		
		float maxlight = Lighting.SHADOWS;
		float tmplight = 0;
		
		// underground additional ray damage
		if (position.y < 64)
		{
			tmplight = 1.0 - position.y / 64;
			maxlight += tmplight * (Lighting.DARKNESS - Lighting.SHADOWS);
			tmplight *= maxlight;
		}
		
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
		
		int posy = (int) position.y;
		int byy;
		
		while (distance_curve >= inv_reach)
		{
			// world bounds check
			if (posy >= max_y) return tmplight;
			
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
						// the ray has died, exit while
						break;
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
				if (distance_curve <= inv_reach) return tmplight;
				
				// big ray step
				posy += byy;
			}
			
		} // while loop
		
		return tmplight;
	}
	
	vertex_color_t LightingClass::lightCheck(LightList& list, Sector& sector, int bx, int by, int bz)
	{
		float tmplight;
		int bxx = bx, byy = by, bzz = bz;
		
		// start from s, and work out possibly new position
		Sector* s2 = Spiders::spiderwrap(sector, bxx, byy, bzz);
		if (s2 == nullptr)
		{
			tmplight = 0.0f;
			goto theend;
		}
		
	/* scope */;
	{
		float radangle = thesun.getRadianAngle();
		float angle_x  = thesun.getAngle().x;
		float angle_y  = thesun.getAngle().y;
		
		if (angle_y < 0)
		{
			angle_y = -angle_y;
			radangle += PI;
		}
		
		vec3 position = vec3(
			s2->x * Sector::BLOCKS_XZ + bxx, 
			s2->y * Sector::BLOCKS_Y  + byy,
			s2->z * Sector::BLOCKS_XZ + bzz
		);
		
		#define sunray   lightRay3D(position, vec3(angle_x,       angle_y, 0.0))
		#define halfray  lightRay3D(position, vec3(angle_x * 0.5, angle_y, 0.0))
		#define skyray   lightRay1D(position)
		
		if (Lighting.ray_count <= 1)
		{
			// towards sun
			tmplight = sunray;
		}
		else if (Lighting.ray_count == 2)
		{
			// towards sun & sky
			tmplight = sunray * 0.6 + skyray * 0.4;
		}
		else if (Lighting.ray_count == 3)
		{
			// towards sun & sky
			tmplight = sunray * 0.5 + halfray * 0.25 + skyray * 0.25;
		}
		else
		{
			// towards sun
			float sunlight = sunray;
			// towards sky
			float skylight = skyray * 0.5 + halfray * 0.5;
			
			if (angle_y > 0.15)
			{
				// move ray to avoid hitting self at low angle
				position.x += angle_x;
				position.y += angle_y;
			}
			
			const float deg15 = PI / 2 / 6;
			const float deg30 = PI / 2 / 3;
			const float cos30 = cosf(deg30);
			const float sin30 = sinf(deg30);
			
			#define bouncelight tmplight
			
			// +/- x
			bouncelight  = lightRay3D(position, vec3(cosf(radangle - deg15), sinf(radangle - deg15), 0.0));
			bouncelight += lightRay3D(position, vec3(cosf(radangle + deg15), sinf(radangle + deg15), 0.0));
			// +/- z
			bouncelight += lightRay3D(position, vec3(sin30 * angle_x, cos30 * angle_y,  sin30 * cos30 * angle_y));
			bouncelight += lightRay3D(position, vec3(sin30 * angle_x, cos30 * angle_y, -sin30 * cos30 * angle_y));
			bouncelight *= 1.0 / 4.0;
			
			tmplight = sunlight * 0.3 + bouncelight * 0.4 + skylight * 0.3;
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
