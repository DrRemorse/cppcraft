#include "worldmanager.hpp"

#include <library/log.hpp>
#include <library/timing/timer.hpp>
#include <library/sleep.hpp>
#include "chunks.hpp"
#include "generatorq.hpp"
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
	static const double MAX_TIMING_WAIT  = 0.012;
	static const double TIMING_SLEEP_TIME = TIMING_TICKTIMER / 2.0;
	
	void WorldManager::main()
	{
		try
		{
			// start me some networking
			network.init(*this);
		}
		catch (std::string exc)
		{
			logger << Log::ERR << "Network::init(): " << exc << Log::ENDL;
		}
		
		// integral delta timing
		Timer timer;
		double localTime = timer.getTime();
		double _ticktimer = localTime;
		
		// world manager main loop
		while (mtx.terminate == false)
		{
			// fixed timestep
			localTime = timer.getTime();
			
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
				double _localtime = localTime;
				
				while (_localtime >= _ticktimer + TIMING_TICKTIMER)
				{
					//----------------------------------//
					//       PLAYER RELATED STUFF       //
					//----------------------------------//
					player.handlePlayerTicks();
					
					// handle actors & particles & objects
					particleSystem.autoCreate();
					particleSystem.update();
					
					// handle sound, music & ambience
					soundman.handleSounds(player.getTerrain());
					
					_ticktimer += TIMING_TICKTIMER;
					
					// fixed timestep
					_localtime = timer.getTime();
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
				player.handleActions(localTime);
			}
			catch (std::string exc)
			{
				logger << Log::ERR << "Error from player.handleActions(): " << exc << Log::ENDL;
				break;
			}
			
			while (true)
			{
				///----------------------------------///
				///        SEAMLESS TRANSITION       ///
				///----------------------------------///
				if (Seamless::run()) break;
				
				///----------------------------------///
				/// ---------- GENERATOR ----------- ///
				///----------------------------------///
				// mandatory to run entire operation
				generatorQueue.run();
				
				double timeOut = localTime + MAX_TIMING_WAIT;
				
				// check for timeout
				if (timer.getTime() > timeOut) break;
				
				// update shadows if sun has travelled far
				// but not when connected to a network
				if (network.isConnected() == false)
				{
					thesun.travelCheck();
				}
				
				///----------------------------------///
				/// --------- PRECOMPILER ---------- ///
				///----------------------------------///
				//double t0 = timer.getTime();
				//double t0 = _localtime;
				
				// as long as not currently 'generating' world:
				// start precompiling sectors
				if (precompq.run(timer, timeOut)) break;
				
				//double t1 = timer.getTime() - t0;
				//if (t1 > 0.020)
				//{
				//	logger << "Precomp delta: " << t1 * 1000 << Log::ENDL;
				//}
				
				//double t1 = timer.getTime();
				//logger << "pcq time: " << t1 - t0 << Log::ENDL;
				
				teleportHandler();
				
				///----------------------------------///
				/// -------- WORLD BUILDER --------- ///
				///----------------------------------///
				// precompq queue must be empty (aka ready for more stuff)
				if (precompq.ready())
				{
					//double t0 = timer.getTime();
					//double t0 = _localtime;
					
					try
					{
						worldbuilder.run(timer, timeOut);
					}
					catch (std::string exc)
					{
						logger << Log::ERR << "Worldbuilder: " << exc << Log::ENDL;
						break;
					}
					
					//double t1 = timer.getTime() - t0;
					//if (t1 > 0.020)
					//{
					//	logger << "Worldbuilder delta: " << t1 * 1000 << Log::ENDL;
					//}
					
					//double t1 = timer.getTime();
					//logger << "WB time: " << t1 - t0 << Log::ENDL;
				}
				
				break;
			} // world builder
			
			// send & receive stuff
			network.handleNetworking(*this);
			
			// flush chunk write queue
			chunks.flushChunks();
			
			// this shit won't work...
			/*if (timer.getDeltaTime() < localTime + TIMING_SLEEP_TIME)
			{
				sleepMillis(6);
			}*/
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
