/**
 * Everyzing ztarts hier
 * 
 * LOC:
 * find . -name '*.?pp' | xargs wc -l | tail -1
 * 
**/

#include "library/config.hpp"
#include "library/log.hpp"
#include "library/math/vector.hpp"
#include "library/opengl/input.hpp"

#include "gameconf.hpp"
#include "generator.hpp"
#include "player.hpp"
#include "renderman.hpp"
#include "threading.hpp"
#include "worldmanager.hpp"

#include <string>

const std::string configFile = "config.ini";
const std::string logFile    = "cppcraft.log";

using namespace library;
using namespace cppcraft;

int main(int argc, char* argv[])
{
	logger.open(logFile);
	
	// start logging
	logger << Log::INFO << "Starting up..." << Log::ENDL;
	
	std::string wfolder = "";
	if (argc > 1)
	{
		wfolder = argv[1];
		logger << Log::INFO << "Using world: " << wfolder << Log::ENDL;
	}
	
	// read config file
	if (config.load("config.ini") == false)
		logger << Log::WARN << "Could not find config file: " << configFile << Log::ENDL;
	
	// read game configuration
	gameconf.init();
	
	// initialize renderer
	Renderer renderer;
	try
	{
		renderer.create("test window");
	}
	catch (std::string errorstring)
	{
		logger.write(Log::ERR, errorstring);
		logger.write(Log::ERR, "Failed to initialize renderer... Exiting.");
		return 1;
	}
	
	// initialize game/world manager
	WorldManager worldman;
	try
	{
		worldman.init(WorldManager::GS_RUNNING, wfolder);
	}
	catch (std::string errorstring)
	{
		logger.write(Log::ERR, errorstring);
		logger.write(Log::ERR, "Failed to initialize renderer... Exiting.");
		return 1;
	}
	
	// initialize input systems
	input.init(renderer.getScreen());
	input.setRotation(vec2(player.xrotrad, player.yrotrad));
	
	try
	{
		// prepare renderer
		renderer.prepare();
	}
	catch (std::string errorstring)
	{
		logger.write(Log::ERR, errorstring);
		logger.write(Log::ERR, "Failed to prepare renderer... Exiting.");
		return 1;
	}
	
	try
	{
		// initialize generator as late as possible because
		// there's some rendering dependencies that are just barely finished
		Generator::init();
	}
	catch (std::string errorstring)
	{
		logger.write(Log::ERR, errorstring);
		logger.write(Log::ERR, "Failed to generate inital world... Exiting.");
		return 1;
	}
	
	try
	{
		// start world manager thread
		mtx.initThreading(worldman);
	}
	catch (std::string errorstring)
	{
		logger.write(Log::ERR, errorstring);
		logger.write(Log::ERR, "Failed to initialize threading... Exiting.");
		return 1;
	}
	
	logger << Log::INFO << "* Starting renderer..." << Log::ENDL;
	
	// get stuck in rendering-loop
	renderer.renderloop(worldman);
	
	logger << Log::INFO << "Ending..." << Log::ENDL;
	
	// cleanup
	mtx.cleanupThreading();
	
	return 0;
}
