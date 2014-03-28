/**
 * Chatbox showing you unimportant text
 * 
**/
#ifndef CHAT_HPP
#define CHAT_HPP

#include <library/opengl/vao.hpp>
#include <mutex>
#include <string>
#include <time.h>
#include <vector>

namespace library
{
	class vec2;
	class vec3;
	class mat4;
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
			L_CHAT
		};
		
		struct ChatLine
		{
			std::string source;
			std::string text;
			chattype_t  type;
			time_t time;
			size_t length;
			bool   newline;
			
			ChatLine(std::string Source, std::string Text, chattype_t Type);
		};
		
		typedef unsigned int color_t;
		
		void init(float width, float height);
		void render(library::SimpleFont& font, const library::mat4& ortho, const library::vec2& textScale, Renderer& renderer);
		void add(const std::string& source, const std::string& text, chattype_t type);
		
		inline void openChat(bool open)
		{
			chatOpen = open;
		}
		inline bool isOpen() const { return chatOpen; }
		
	private:
		float fadeout;
		library::VAO cbvao;
		
		std::vector<ChatLine> lines;
		std::mutex  mtx;
		bool chatOpen;
		
		void renderSourcedMessage
		(
			library::SimpleFont& font, 
			const library::vec3& spos, 
			const library::vec2& scale, 
			const std::string& time, 
			const std::string& source, 
			const std::string& text,
			float alpha
		);
		void renderInfoMessage
		(
			library::SimpleFont& font, 
			const library::vec3& spos, 
			const library::vec2& scale, 
			const std::string& time, 
			const std::string& from, 
			const std::string& text,
			float alpha
		);
		void renderMessage
		(
			library::SimpleFont& font, 
			const library::vec3& spos, 
			const library::vec2& scale, 
			const std::string& time, 
			const std::string& text,
			float alpha
		);
	};
	extern Chatbox chatbox;
}

#endif
