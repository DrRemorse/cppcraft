#ifndef SEAMLESS_HPP
#define SEAMLESS_HPP

namespace cppcraft
{
	
	class Seamless
	{
	private:
		static bool seamlessness();
		static void seamless_preconditions();
		
	public:
		static bool run();
		
		static const int OFFSET;
	};
	
}

#endif
