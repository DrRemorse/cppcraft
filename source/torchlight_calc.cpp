#include "torchlight.hpp"

#include "library/log.hpp"
#include "library/bitmap/colortools.hpp"
#include "library/math/vector.hpp"
#include "lighttable.hpp"
#include "precompq.hpp"
#include "sectors.hpp"
#include <cmath>

#define OCCLUSION
#define SIMPLE_OCCLUSION

using namespace library;

namespace cppcraft
{
	void Torchlight::lightSectorUpdates(Sector& s, block_t id, bool instant)
	{
		#define L_G_R  LIGHT_GATHER_REACH
		
		int ysize = L_G_R;
		if (getEmitter(getEmitterId(id)).reach > Sector::BLOCKS_Y)
		{
			ysize *= 2;
		}
		
		// create boundries
		int x0 = (s.x - L_G_R < 0) ? 0 : s.x - L_G_R;
		int x1 = (s.x + L_G_R >= Sectors.getXZ()) ? Sectors.getXZ()-1 : s.x + L_G_R;
		int y0 = (s.y - ysize < 0) ? 0 : s.y - ysize;
		int y1 = (s.y + ysize >= Sectors.getY()) ? Sectors.getY()-1 : s.y + ysize;
		int z0 = (s.z - L_G_R < 0) ? 0 : s.z - L_G_R;
		int z1 = (s.z + L_G_R >= Sectors.getXZ()) ? Sectors.getXZ()-1 : s.z + L_G_R;
		
		// force regeneration of affected sectors
		int x = x0, y = y0, z = z0;
		
		while (true)
		{
			Sector& ss = Sectors(x, y, z);
			
			// set lights flag: unknown number of gatherable lights
			ss.haslight = 0;
			
			if (ss.render)
			{
				// as long as not already in precompiler queue (??)
				if (ss.progress != Sector::PROG_RECOMP)
				{
					ss.progress = Sector::PROG_NEEDRECOMP;
					if (instant) precompq.addTruckload(ss);
				}
			}
			
			z += 1;
			if (z > z1)
			{
				y += 1;
				if (y > y1)
				{
					x += 1;
					if (x > x1) break;
					y = y0;
				}
				z = z0;
			}
		}
	}
	
	// --------- TORCHLIGHT OCCLUSION FUNCTIONS --------- //
	
	int Torchlight::OcclusionComp(block_t id)
	{
		return isAir(id) || (id > LOWBLOCK_END);
	}
	
	float Torchlight::getLightDistance(blocklight_t& light, Sector& sector, int bx, int by, int bz)
	{
		Sector& L = *light.s;
		vec3 position(light.bx + 0.5 - bx,
					  light.by + 0.5 - by,
					  light.bz + 0.5 - bz);
		// same sector, no calculations
		if (L != sector)
		{
			// different sector
			position.x += (L.x - sector.x) * Sector::BLOCKS_XZ;
			position.y += (L.y - sector.y) * Sector::BLOCKS_Y;
			position.z += (L.z - sector.z) * Sector::BLOCKS_XZ;
		}
		
		//if (light.id == _TORCH || light.id == _LANTERN)
		//	CalculateOcclusion()
		//else
		//logger << getEmitter(light.id).reach << " vs distance: " << position.length() << Log::ENDL;
		return getEmitter(light.id).reach - position.length();
	}
	
	vertex_color_t Torchlight::torchlight(LightList& list, float shadowLevel, Sector& sector, int bx, int by, int bz)
	{
		float L = 1.0 - (long)shadowLevel / 255.0;
		// create base light value from shadow level
		vec4  vtorch(L, L, L, 0.0);
		float brightness = 0.0;
		
		for (size_t i = 0; i < list.lights.size(); i++)
		{
			L = getLightDistance(list.lights[i], sector, bx, by, bz); // inverse distance
			if (L > 0.01)
			{
				// light was in range
				const lightdata_t& light = getEmitter(list.lights[i].id);
				
				float intensity = powf(L / light.reach, light.curve);
				
				if (intensity > 0.01)
				{
					intensity *= light.cap;
					
					vtorch.x = vtorch.x * (1.0 - intensity) + light.r * intensity;
					vtorch.y = vtorch.y * (1.0 - intensity) + light.g * intensity;
					vtorch.z = vtorch.z * (1.0 - intensity) + light.b * intensity;
					vtorch.w = vtorch.w * (1.0 - intensity) + intensity * light.colors;
					
					brightness = brightness * (1.0 - intensity) + intensity * light.brightness;
				}
			}
		} // next light
		
		// set shadow, brightness and cornershadow defaults
		vertex_color_t retval = vertex_color_t(255 - shadowLevel) +
							   (vertex_color_t(brightness * 255) << 8) +
							   (255 << 16);
		// set torchlight color
		*((unsigned int*) &retval+1) = BGRA8(vtorch.x * 255,
											 vtorch.y * 255,
											 vtorch.z * 255,
											 vtorch.w * 255);
		return retval;
	}
	
}

/*
#Macro CalculateOcclusion()
	dim as single dist = Sqr(dbx*dbx + dby*dby + dbz*dbz)
	dim as single reach = globalLightData( .id ).reach
	
	'' test if same light source
	if l = s and .bx = bx and .by = by and .bz = bz then
		return reach - dist
	endif
	
	#ifdef OCCLUSION
		If dist >= reach Then Return 0.0
		
		Dim As Single visibility = 1.0 - dist / reach
		
		Dim As Single stepsize = 1.0 / dist
		
		Const As Single tlv_damage = 2.0 '' ramp
		
		#ifndef SIMPLE_OCCLUSION
			
			dim as short max_x = dbx
			dim as short max_y = dby
			dim as short max_z = dbz
			
			'' direction
			dim as single nx = 1.0 / (max_x * stepsize), maxx = 0.0
			dim as single ny = 1.0 / (max_y * stepsize), maxy = 0.0
			dim as single nz = 1.0 / (max_z * stepsize), maxz = 0.0
			
			'' position
			dim as short stepx = iif(nx < 0, -1, 1)
			dim as short stepy = iif(ny < 0, -1, 1)
			dim as short stepz = iif(nz < 0, -1, 1)
			
			dim as short dx = 0, dy = 0, dz = 0
			
			'' voxel traversal
			while true
				
				if maxx < maxy then
					
					if maxx < maxz then
						
						'' x step
						dx += stepx
						if dx > max_x then exit while
						maxx += nx
						
					else
						
						'' z step
						dz += stepz
						if dz > max_z then exit while
						maxz += nz
					
					endif
					
				else '' y < x
					
					if maxy < maxz then
						
						'' y step
						dy += stepy
						if dy > max_y then exit while
						maxy += ny
						
					else
						
						'' z step
						dz += stepz
						if dz > max_z then exit while
						maxz += nz
						
					endif
					
				endif
				
				if OcclusionComp( getblocklight(s, bx + dx, by + dy, bz + dz) ) = FALSE then
					visibility ^= tlv_damage
					if visibility < 0.05 then return visibility * reach
				endif
				
			wend
			
		#else
			
			dim as short fbx = int(bx + dbx * stepsize)
			dim as short fby = int(by + dby * stepsize)
			dim as short fbz = int(bz + dbz * stepsize)
			
			'towards light source
			if s = l then
				'' same sector, source can be dest!
				
				if fbx <> .bx Or fby <> .by Or fbz <> .bz then
					
					'if OcclusionComp( s->blockpt->b(fbx, fbz, fby).id ) = FALSE then
					if OcclusionComp( getblocklight(s, fbx, fby, fbz) ) = FALSE then
						visibility ^= tlv_damage
					endif
					
				endif
			else
				'' different sector, definitely NOT source = dest
				
				if OcclusionComp( getblocklight(s, fbx, fby, fbz) ) = FALSE then
					visibility ^= tlv_damage
				endif
				
			endif
			
			'from light source
			fbx = cshort(.bx - dbx * stepsize) 'cshort!!
			fby = cshort(.by - dby * stepsize)
			fbz = cshort(.bz - dbz * stepsize)
			
			if s = l then
				'' same sector, source can be dest!
				
				if fbx <> bx Or fby <> by Or fbz <> bz then
					
					'' NOTE: since a light is at 0.5, 0.5, 0.5
					'' its possible to end up at blocksz for X, Z etc.
					
					'' due to rounding errors, need to use getblocklight
					if OcclusionComp( getblocklight(l, fbx, fby, fbz) ) = FALSE Then
						visibility ^= tlv_damage
					endif
					
				endif
				
			else
				'' different sector, definitely NOT source = dest
				
				if OcclusionComp( getblocklight(l, fbx, fby, fbz) ) = FALSE then
					visibility ^= tlv_damage
				endif
				
			endif
		#endif
		
		return visibility * reach
	#Else
		return reach - dist
	#EndIf
	
#EndMacro
*/
