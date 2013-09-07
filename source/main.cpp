/**
 * Everyzing ztarts hier
 * 
 * LOC:
 * find . -name '*.?pp' | xargs wc -l | tail -1
 * 
**/

#include "library/config.hpp"
#include "library/log.hpp"
#include "library/math/matrix.hpp"

#include "gameconf.hpp"
#include "generator.hpp"
#include "input.hpp"
#include "renderman.hpp"
#include "threading.hpp"
#include "worldmanager.hpp"

#include "vertex_block.hpp"

#include <string>

const std::string configFile = "config.ini";
const std::string logFile    = "cppcraft.log";

using namespace library;

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
	cppcraft::gameconf.init();
	
	// initialize renderer
	cppcraft::Renderer renderer;
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
	cppcraft::WorldManager worldman;
	try
	{
		worldman.init(cppcraft::WorldManager::GS_RUNNING, wfolder);
	}
	catch (std::string errorstring)
	{
		logger.write(Log::ERR, errorstring);
		logger.write(Log::ERR, "Failed to initialize renderer... Exiting.");
		return 1;
	}
	
	// initialize input systems
	cppcraft::input.init(renderer.getScreen());
	
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
		cppcraft::Generator::init();
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
		cppcraft::mtx.initThreading(worldman);
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
	cppcraft::mtx.cleanupThreading();
	
	return 0;
}
