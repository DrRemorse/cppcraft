#ifndef CAMERA_FRUSTUM_HPP
#define CAMERA_FRUSTUM_HPP

#include "library/opengl/camera.hpp"

namespace cppcraft
{
	class Camera : public library::Camera
	{
	private:
		int gridsize;
		
		// the viewing frustum
		library::Frustum frustum;
		
		// camera matrices
		library::Matrix matproj_long; // PL
		
		// in-sector offset (from seamless transition)
		library::vec3 worldOffset;
		
	public:
		bool recalc;   // recalculate viewing frustum
		bool rotated;  // camera was rotated, resend rotation matrices
		bool ref;      // run occlusion tests
		int  needsupd; // update render list
		int  cameraViewSectors;
		
		void init(library::WindowClass& wnd);
		
		int getGridsize() const { return this->gridsize; }
		const library::Matrix& getProjectionLong() const;
		
		inline const library::vec3& getWorldOffset() const
		{
			return this->worldOffset;
		}
		void setWorldOffset(double posX, double posZ);
		
	};
	extern Camera camera;
}

#endif
