/**
 * Chatbox showing you unimportant text
 * 
**/
#ifndef CHAT_HPP
#define CHAT_HPP

#include <mutex>
#include <string>
#include <time.h>
#include <vector>

namespace library
{
	class SimpleFont;
}

namespace cppcraft
{
	class Renderer;
	extern time_t currentTime();
	
	class Chatbox
	{
	public:
		enum chattype_t
		{
			L_INFO,
			L_SERVER,
			L_SELF,
			L_CHAT
		};
		
		struct ChatLine
		{
			std::string source;
			std::string text;
			chattype_t  type;
			time_t time;
			bool newline;
			
			ChatLine(std::string Text, chattype_t Type) :
				source(""), text(Text), type(Type), time(currentTime()) {}
			ChatLine(std::string Source, std::string Text, chattype_t Type) :
				source(Source), text(Text), type(Type), time(currentTime()) {}
		};
		
		typedef unsigned int color_t;
		
		Chatbox();
		
		void init();
		void render(library::SimpleFont& font, Renderer& renderer);
		void add(const std::string& text, chattype_t type);
		void add(const std::string& source, const std::string& text, chattype_t type);
		
	private:
		float fadeout;
		
		std::vector<ChatLine> lines;
		std::string typetext;
		std::mutex  mtx;
		bool typing;
	};
	extern Chatbox chatbox;
}

#endif
