#ifndef LOG_HPP
#define LOG_HPP

#include <fstream>
#include <string>
#include <sstream>
#include <mutex>

/**
 * Thread-safe logger class, logs to file & console/stdout (if enabled)
 * 
 * NOTE: Project must have declared/instantiated engine::Log engine::logger
 * somewhere in the project for this file to compile properly
 * 
 * To do that add this to main.cpp just after including log.hpp:
 * 	  engine::Log engine::logger("my.log.file");
 * 
 * To disable console/stdout ouputput:
 * logger.setOutputConsole(false);
 * 
 * To inform about initialization/progression:
 * logger << Log::INFO << "* Sound system initialized" << Log::ENDL;
 * 
 * To log an error:
 * logger << Log::ERR << "MyClass::myFunction(): Something bad happened: " << error_number << Log::ENDL;
 * 
**/

namespace library
{
	class Log
	{
		public:
			typedef enum{ ENDL, INFO, WARN, ERR } log_level_t;
			
			Log();
			Log(std::string);
			~Log();
			void open(std::string);
			bool write(log_level_t, std::string);
			
			void setOutputConsole(bool console);
			
		private:
			std::mutex synch;
			std::string log;
			std::ofstream file;
			
			bool outputConsole;
			
			friend Log& operator<< (Log& out, const log_level_t level);
			template <class T>
			friend Log& operator<< (Log& out, const T t);
	};
	
	// Log << T
	template <class T>
	Log& operator<< (Log& out, const T t)
	{
		std::stringstream ss;
		ss << t;
		out.log += ss.str();
		return out;
	}
	
	extern Log logger;
	
}

#endif
