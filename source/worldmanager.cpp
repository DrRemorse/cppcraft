#include "worldmanager.hpp"

#include <library/log.hpp>
#include <library/timing/timer.hpp>
#include <library/sleep.hpp>
#include "chunks.hpp"
#include "network.hpp"
#include "particles.hpp"
#include "player.hpp"
#include "precompq.hpp"
#include "seamless.hpp"
#include "soundman.hpp"
#include "sun.hpp"
#include "threading.hpp"
#include "worldbuilder.hpp"
#include "world.hpp"

using namespace library;

namespace cppcraft
{
	static const double TIMING_TICKTIMER = 0.0125;
	static const double MAX_TIMING_WAIT  = 0.011;
	
	void WorldManager::main()
	{
		try
		{
			// start me some networking
			network.init();
		}
		catch (std::string exc)
		{
			logger << Log::ERR << "Network::init(): " << exc << Log::ENDL;
		}
		
		// integral delta timing
		Timer timer;
		double _localtime = timer.getDeltaTime();
		double _ticktimer = timer.getDeltaTime();
		
		// world manager main loop
		while (mtx.terminate == false)
		{
			// fixed timestep
			_localtime = timer.getDeltaTime();
			
			try
			{
				// handle player inputs, just once
				player.handleInputs();
			}
			catch (std::string exc)
			{
				logger << Log::ERR << "Error from player.handleInputs(): " << exc << Log::ENDL;
				break;
			}
			
			// integrator
			try
			{
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
			}
			catch (std::string exc)
			{
				logger << Log::ERR << "Error from worldmanager ticker: " << exc << Log::ENDL;
				break;
			}
			
			try
			{
				/// if the player moved, or is currently doing stuff we will be doing it here ///
				
				// this function changes the player.positionChanged value each round
				// also transports relevant player values to rendering thread
				player.handleActions(_localtime);
			}
			catch (std::string exc)
			{
				logger << Log::ERR << "Error from player.handleActions(): " << exc << Log::ENDL;
				break;
			}
			
			
			///----------------------------------///
			///        SEAMLESS TRANSITION       ///
			///----------------------------------///
			bool timeout = Seamless::run();
			
			// check for timeout
			if (timer.getDeltaTime() > _localtime + MAX_TIMING_WAIT) timeout = true;
			
			// update shadows if sun travelled far
			thesun.travelCheck();
			
			///----------------------------------///
			/// --------- PRECOMPILER ---------- ///
			///----------------------------------///
			if (timeout == false)
			if (worldbuilder.getMode() != worldbuilder.MODE_GENERATING)
			{
				//double t0 = timer.getDeltaTime();
				//double t0 = _localtime;
				
				try
				{
					// as long as not currently 'generating' world:
					// start precompiling sectors
					timeout = precompq.run(timer, _localtime);
				}
				catch (std::string exc)
				{
					logger << Log::ERR << "PrecompQ: " << exc << Log::ENDL;
					break;
				}
				
				//double t1 = timer.getDeltaTime() - t0;
				//if (t1 > 0.020)
				//{
				//	logger << "Precomp delta: " << t1 * 1000 << Log::ENDL;
				//}
				
				//double t1 = timer.getDeltaTime();
				//logger << "pcq time: " << t1 - t0 << Log::ENDL;
			}
			
			///----------------------------------///
			/// -------- WORLD BUILDER --------- ///
			///----------------------------------///
			if (timeout == false)
			{
				// either the worldbuilder is not in precompiler queue-adding mode
				// or precompq queue must be empty (aka ready for more stuff)
				if (worldbuilder.getMode() != WorldBuilder::MODE_PRECOMPILE || precompq.ready())
				{
					//double t0 = timer.getDeltaTime();
					//double t0 = _localtime;
					
					try
					{
						worldbuilder.run(timer, _localtime);
					}
					catch (std::string exc)
					{
						logger << Log::ERR << "Worldbuilder: " << exc << Log::ENDL;
						break;
					}
					
					//double t1 = timer.getDeltaTime() - t0;
					//if (t1 > 0.020)
					//{
					//	logger << "Worldbuilder delta: " << t1 * 1000 << Log::ENDL;
					//}
					
					//double t1 = timer.getDeltaTime();
					//logger << "WB time: " << t1 - t0 << Log::ENDL;
				}
			} // world builder
			
			// send & receive stuff
			network.handleNetworking();
			
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
