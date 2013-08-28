#ifndef ATMOSPHERE_HPP
#define ATMOSPHERE_HPP

namespace library
{
	class Matrix;
}

namespace cppcraft
{
	class Atmosphere
	{
	public:
		static void init();
		static void render(float playerY);
		
	};
}

#endif
