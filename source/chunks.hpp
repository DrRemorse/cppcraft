#ifndef CHUNKS_HPP
#define CHUNKS_HPP

/**
 * Chunk files,
 * compressed and uncompressed
 * 
 * Sectors can be added to a write queue, which then causes that sector
 * to be dumped to its base32 file.
 * 
 * Chunk loader will try to read as many sectors in one go as possible
 * each time the "generator" is running.
 * 
**/

#include <string>
#include <vector>
#include <fstream>

namespace cppcraft
{
	class Generator;
	class Sector;
	
	class Chunk
	{
	public:
		// sector queue: sectors pending being written to disk
		std::vector<Sector*> writeq;
		// chunk token, unique identifier
		int tokenX, tokenZ;
	};
	
	class Chunks
	{
	public:
		static const int chunk_size = 32;
		
		// chunks
		void initChunks();
		void flushChunks();
		
		// sectors
		std::string getSectorString(Sector& s);
		void addSector(Sector& sector);
		
	private:
		void writeChunk(Chunk&);
		void writeSector(Sector& s, std::fstream& File);
		void loadSector(Sector& sector, std::ifstream&, unsigned int position);
		
		// special data
		//int createSectorData(Sector* s, short bx, short by, short bz, int id);
		//void writeSpecialData(Sector* s, filetoken);
		//void loadSpecialData(Sector* s, int dx, int dy, int dz);
		//void removeSpecialData(Sector* s, int id, int index);
		
		friend Generator; // Generator can access private functions
		
		// chunk queue: chunks pending being written to disk
		std::vector<Chunk> chunkq;
	};
	extern Chunks chunks;
	
}

#endif
