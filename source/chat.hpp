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
	public:
		typedef unsigned int color_t;
		
		void render();
		void add(std::string text, color_t color, bool newline);
		void addChat(std::string source, color_t scolor, std::string text, color_t tcolor);
		
	private:
		struct ChatboxLine
		{
			std::string text;
			color_t color;
			bool newline;
			
			ChatboxLine() {}
			ChatboxLine(std::string Text, color_t Color, bool NewLine) :
				text(Text), color(Color), newline(NewLine) {}
		};
		
		std::vector<ChatboxLine> lines;
		std::string typetext;
		bool typing;
	};
	extern Chatbox chatbox;
}

#endif
