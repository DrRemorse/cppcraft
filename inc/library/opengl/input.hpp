#ifndef INPUT_HPP
#define INPUT_HPP

#include <library/math/vector.hpp>
#include <string>

struct GLFWwindow;

namespace library
{
	class WindowClass;
	
	class Input
	{
	public:
		typedef enum
		{
			KEY_RELEASED,
			KEY_PRESSED,
			KEY_LOCKED
			
		} keystate_t;
		
		typedef int key_t;
		
		struct input_t
		{
			input_t() : action(0), mods(0) {};
			
			int action;
			int mods;
		};
		
		// initialize input using the current context
		void init(library::WindowClass& gamescr, bool keyboard, bool mouse);
		
		// public keyboard functions
		const input_t& getKeyEx(int) const;
		key_t getKey(int) const;
		// "holds" a key, allowing code to see that a single-fire event has been executed already
		void hold(int);
		// releases the hold of a key, setting state from KEY_LOCKED back to KEY_PRESSED
		void release(int);
		
		// public mouse functions
		// set mouse options
		void mouseOptions(double speed, double sensitivity);
		// enable FPS-like mouse grabbing
		void grabMouse(bool grab);
		// mouse position
		inline const vec2& getMousePos() const
		{
			return mousePos;
		}
		inline const vec2& getMouseLastPos() const
		{
			return lastMousePos;
		}
		const input_t& getMouseEx(int) const;
		key_t getMouse(int) const;
		void  holdMouse(int);
		// rotation functions
		inline const vec2& getRotation() const noexcept
		{
			return this->rotation;
		}
		inline void setRotation(const vec2& newRotation)
		{
			this->rotation = newRotation;
		}
		// returns mousewheel status _AND_ resets it internally
		int getWheel(); // 0 = no change, down < 0, up > 0
		
		
		inline const std::string& getText() const
		{
			return this->text;
		}
		bool textBackspace();
		inline void clearText()
		{
			this->text.clear();
		}
		
	private:
		WindowClass* gamescr;
		double speed;
		double sensitivity;
		vec2   mousePos;
		vec2   lastMousePos;
		bool   mousegrab;
		
		// pitch & yaw
		vec2 rotation;
		
		// keyboard keys
		static const int MAX_KEYS = 512;
		input_t keys[MAX_KEYS];
		// mouse buttons
		static const int MAX_MOUSE = 3;
		input_t mouse[MAX_MOUSE];
		// mousewheel
		int wheel;
		// typed text
		std::string text;
		
		friend void keyboard(GLFWwindow* window, int key, int action, int a, int b);
		friend void keyboardType(GLFWwindow* window, unsigned int character);
		friend void mouseMove(GLFWwindow* window, double x, double y);
		friend void mouseButton(GLFWwindow* window, int button, int action, int mods);
		friend void mouseWheel(GLFWwindow* window, double x, double y);
	};
	extern Input input;
}

#endif
