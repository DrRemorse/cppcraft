/**
 * 
 * 
 * 
**/

#ifndef CHAT_HPP
#define CHAT_HPP

#include <vector>
#include <string>

namespace cppcraft
{
	
	class Chatbox
	{
		typedef unsigned int color_t;
		
		struct ChatboxLine
		{
			std::string text;
			unsigned int color;
			bool newline;
		};
		
		std::vector<ChatboxLine> lines;
		std::string typetext;
		
		int chatmode;
		
		void render();
		void add(std::string text, color_t color, bool newline);
		void addChat(std::string source, color_t scolor, std::string text, color_t tcolor);
	};
	extern Chatbox chatbox;
}

#endif
