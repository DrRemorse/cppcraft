#include "torchlight.hpp"

#include "blocks.hpp"
#include <string>
#include <cmath>

namespace cppcraft
{
	Torchlight torchlight;
	
	void Torchlight::init()
	{
		lightdata_t* p;
		
		p = &torchlights[L_FOG];
		// fog (_FOG)
		p->reach = 16;
		p->curve = 0.6;
		p->cap   = 0.25;
		
		p->colors     = 0.15;
		p->brightness = 2.0;
		
		p->r = 1.0;
		p->g = 1.0;
		p->b = 1.0;
		
		p = &torchlights[L_TORCH];
		// torch (_TORCH)
		p->reach = 8;
		p->curve = 1.2;
		p->cap   = 1.0;
		
		p->colors     = 0.2;
		p->brightness = 0.8;
		
		p->r = 1.0;
		p->g = 1.0;
		p->b = 1.0;
		
		p = &torchlights[L_LAVA];
		// lava (_LAVABLOCK)
		p->reach = 5;
		p->curve = 1.0;
		p->cap   = 0.5;
		
		p->colors     = 0.5;
		p->brightness = 0.8;
		
		p->r = 1.0;
		p->g = 0.45;
		p->b = 0.1;
		
		p = &torchlights[L_REDSTONE];
		// redstone (_REDSTONE)
		p->reach = 6;
		p->curve = 1.4; // exponential
		p->cap   = 0.9; // multiplicative
		
		p->colors     = 0.5;
		p->brightness = 0.3;
		
		p->r = 0.9;
		p->g = 0.0;
		p->b = 0.0;
		
		p = &torchlights[L_GREENSTN];
		// magical green stone (_GREENSTONE)
		p->reach = 6;
		p->curve = 1.6;
		p->cap   = 0.8;
		
		p->colors     = 0.5;
		p->brightness = 0.3;
		
		p->r = 0.0;
		p->g = 0.9;
		p->b = 0.0;
		
		p = &torchlights[L_FIRE];
		// cross fire (_FIRE)
		p->reach = 6;
		p->curve = 2.0;
		p->cap   = 0.8;
		
		p->colors     = 0.5;
		p->brightness = 0.5;
		
		p->r = 1.0;
		p->g = 0.85;
		p->b = 0.15;
		
		p = &torchlights[L_LANTERN];
		// pyramidal lantern (_LANTERN)
		p->reach = 8;
		p->curve = 1.2;
		p->cap   = 1.0;
		
		p->colors     = 0.25;
		p->brightness = 1.0;
		
		p->r = 1.0;
		p->g = 0.8;
		p->b = 0.3;
		
		p = &torchlights[L_MOLTEN];
		// molten stones (_MOLTENSTONE)
		p->reach = 3;
		p->curve = 2.0;
		p->cap   = 0.6;
		
		p->colors     = 0.0;
		p->brightness = 0.6;
		
		p->r = 0.6;
		p->g = 0.3;
		p->b = 0.0;
		
	}
	
	int Torchlight::getEmitterId(block_t id) const
	{
		switch (id)
		{
		case _FOG:
			return L_FOG;
			
		case _TORCH:
			return L_TORCH;
			
		case _LAVABLOCK:
			return L_LAVA;
			
		case _REDSTONE:
			return L_REDSTONE;
			
		case _GREENSTONE:
			return L_GREENSTN;
			
		case _FIRE:
			return L_FIRE;
			
		case _LANTERN:
			return L_LANTERN;
			
		case _MOLTENSTONE:
			return L_MOLTEN;
			
		default:
			throw std::string("Torchlight::getEmitter(): No existing conversion from block to emitter id");
		}
	}
	
	const Torchlight::lightdata_t& Torchlight::getEmitter(int id) const
	{
		return this->torchlights[id];
	}
	
	float Torchlight::getModulation(double frameCounter)
	{
		const float TL_MODULATION_STRENGTH = 0.01;
		float modulation = 0.75 * cos( frameCounter * 0.0085 ) + 1.25 * sin( frameCounter * 0.012 );
		
		return 1.0 + modulation * TL_MODULATION_STRENGTH;
	}
	
}
