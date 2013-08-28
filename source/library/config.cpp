#include "config.hpp"

#include <fstream>
#include "log.hpp"

namespace library
{	
	Config config; // main config file
	
	Config::Config() {}
	
	Config::Config(std::string file)
	{
		load(file);
	}
	
	// load from file
	bool Config::load(std::string file)
	{
		std::fstream filestream;
		filestream.open(file.c_str(), std::fstream::in);
		
		if (!filestream) return false; // could not open file
		
		int numkeys = 0;
		
		// read one line at a time until end of file
		while(!filestream.eof())
		{
			char line[256];
			filestream.getline(line, 256);
			std::string cline(line);
			std::string key, value;
			bool keypart = true;
			
			// iterate config line (string)
			for (std::string::iterator it = cline.begin(); it != cline.end(); ++it)
			{
				if(*it == '#') break; // stop at comment character
				
				if(*it == '=')
				{
					keypart = false;
				}
				else if(*it != ' ') // ignore spaces
				{
					if(keypart)
						key += *it;
					else
						value += *it;
				}
			}
			if(key.length())
			{
				kv.insert(std::pair<std::string,std::string> (key, value));
				numkeys++;
			}
		}
		filestream.close();
		
		logger << Log::INFO << "*** Loaded " << numkeys << " variables from config." << Log::ENDL;
		return true;
	}
	
}
