/*
	Standard generator
*/
#include "../../generator.h"
#include "../../biome/biome.hpp"
#include "generator.hpp"
#include "preproc.hpp"
#include "process.hpp"
#include "postproc.hpp"

void testGen()
{
	logText("-- stdGen 2D --");
	
	// generate biomes
	generate((void*) &biomeGenerator, GEN_TRUE);
	
	logText("-- stdGen 3D --");
	
	// generate land
	generate((void*) &terrainGenerator, GEN_FALSE);
	
	logText("-- done --");
	
}

void finalpassBiome()
{
	// generate biomes
	generate((void*) &biomeGenerator, GEN_FALSE);
	// post-processing (after objects)
	generate((void*) &postPostProcess, GEN_FALSE);
	
}

void testPP()
{
	logText("-- stdPP --");
	
	// pre-processing
	generate((void*) &preProcess, GEN_FALSE);
	// post-processing
	generate((void*) &areaPostProcess, GEN_FALSE);
	// post-processing (after objects)
	generate((void*) &postPostProcess, GEN_FALSE);
	
	logText("-- done --");
}
