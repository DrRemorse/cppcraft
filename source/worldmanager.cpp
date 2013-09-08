#include "worldmanager.hpp"

#include "library/log.hpp"
#include "library/timing/timer.hpp"
#include "chunks.hpp"
#include "particles.hpp"
#include "player.hpp"
#include "precompq.hpp"
#include "seamless.hpp"
#include "soundman.hpp"
#include "threading.hpp"
#include "worldbuilder.hpp"
#include "world.hpp"

using namespace library;

namespace cppcraft
{
	const double TIMING_TICKTIMER = 0.0125;
	const double MAX_TIMING_WAIT  = 0.0125;
	
	void WorldManager::main()
	{
		// integral delta timing
		Timer timer;
		double _localtime = timer.getDeltaTime();
		double _ticktimer = timer.getDeltaTime();
		
		// world manager main loop
		while (mtx.terminate == false)
		{
			// variable delta frame timing
			// 
			_localtime = timer.getDeltaTime();
			
			// handle player inputs, just once
			player.handleInputs();
			
			// integrator
			while (_localtime >= _ticktimer + TIMING_TICKTIMER)
			{
				//----------------------------------//
				//       PLAYER RELATED STUFF       //
				//----------------------------------//
				player.handlePlayerTicks();
				
				// handle actors & particles & objects
				particleSystem.handle();
				
				// handle sound, music & ambience
				soundman.handleSounds(player.getTerrain());
				
				_ticktimer += TIMING_TICKTIMER;
			}
			
			// if the player moved, or is doing stuff we will be doing it here
			player.handleActions(_localtime);
			
			bool timeout = false;
			
			//----------------------------------//
			//        SEAMLESS TRANSITION       //
			//----------------------------------//
			if (Seamless::run()) timeout = true;
			
			// check for timeout
			//if (timer.getDeltaTime() > _localtime + MAX_TIMING_WAIT) goto theend;
			
			// ---------- PRECOMPILER ----------- //
			
			if (timeout == false)
			if (worldbuilder.getMode() != worldbuilder.MODE_GENERATING)
			{
				//double t0 = timer.getDeltaTime();
				
				// as long as not currently 'generating' world:
				// start precompiling sectors
				if (precompq.run(timer, _localtime)) timeout = true;
				
				//double t1 = timer.getDeltaTime();
				//logger << "pcq time: " << t1 - t0 << Log::ENDL;
			}
			
			// ----------- WORLD BUILDER ------------ //
			if (timeout == false)
			{
				//double t0 = timer.getDeltaTime();
				
				worldbuilder.run(timer, _localtime);
				
				//double t1 = timer.getDeltaTime();
				//logger << "WB time: " << t1 - t0 << Log::ENDL;
			}
			
			// flush chunk write queue
			chunks.flushChunks();
			
		}
		
		// flush if queue still exists
		chunks.flushChunks();
		
		// stop precompq
		precompq.stop();
		
		// disconnect if still connected
		#ifdef USE_INET
			networkDisconnect();
		#endif
		
		// save our stuff!
		world.save();
	}
}