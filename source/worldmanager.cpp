#include "worldmanager.hpp"

#include "library/log.hpp"
#include "library/timing/timer.hpp"
#include "chunks.hpp"
#include "network.hpp"
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
	const double MAX_TIMING_WAIT  = 0.01;
	
	void WorldManager::main()
	{
		// start me some networking
		network.init();
		
		// integral delta timing
		Timer timer;
		double _localtime = timer.getDeltaTime();
		double _ticktimer = timer.getDeltaTime();
		
		// world manager main loop
		while (mtx.terminate == false)
		{
			// fixed timestep
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
				
				// fixed timestep
				_localtime = timer.getDeltaTime();
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
				double t0 = _localtime;
				
				// as long as not currently 'generating' world:
				// start precompiling sectors
				if (precompq.run(timer, _localtime)) timeout = true;
				
				double t1 = timer.getDeltaTime() - t0;
				if (t1 > 0.020)
				{
					//logger << "Precomp delta: " << t1 * 1000 << Log::ENDL;
				}
				
				//double t1 = timer.getDeltaTime();
				//logger << "pcq time: " << t1 - t0 << Log::ENDL;
			}
			
			// ----------- WORLD BUILDER ------------ //
			if (timeout == false)
			{
				//double t0 = timer.getDeltaTime();
				double t0 = _localtime;
				
				worldbuilder.run(timer, _localtime);
				
				double t1 = timer.getDeltaTime() - t0;
				if (t1 > 0.020)
				{
					//logger << "Worldbuilder delta: " << t1 * 1000 << Log::ENDL;
				}
				
				//double t1 = timer.getDeltaTime();
				//logger << "WB time: " << t1 - t0 << Log::ENDL;
			}
			
			// flush chunk write queue
			chunks.flushChunks();
			
		}
		
		// flush if queue still exists
		chunks.flushChunks();
		
		// save our stuff!
		world.save();
		
		// stop precompq
		precompq.stop();
		
		// stop & wait for network thread
		network.stop();
		
	}
}
