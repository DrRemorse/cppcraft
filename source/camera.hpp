#ifndef CAMERA_FRUSTUM_HPP
#define CAMERA_FRUSTUM_HPP

#include <library/opengl/camera.hpp>

namespace cppcraft
{
	class Camera : public library::Camera
	{
	private:
		int gridsize;
		
		// the viewing frustum
		library::Frustum frustum;
		
		// camera matrices
		library::mat4 matproj_long; // PL
		
		// in-sector offset (from seamless transition)
		library::vec3 worldOffset;
		library::vec2 nearPlaneHalfSize;
		
	public:
		bool recalc;   // recalculate viewing frustum
		bool rotated;  // camera was rotated, resend rotation matrices
		bool ref;      // run occlusion tests
		int  needsupd; // update render list
		int  cameraViewSectors;
		
		void init(library::WindowClass& wnd);
		
		int getGridsize() const { return this->gridsize; }
		const library::mat4& getProjectionLong() const;
		// deferred helper
		const library::vec2& getNearPlaneHalfSize() const;
		
		inline const library::vec3& getWorldOffset() const
		{
			return this->worldOffset;
		}
		void setWorldOffset(double posX, double posZ);
		
	};
	extern cppcraft::Camera camera;
	extern cppcraft::Camera reflectionCamera;
}

#endif
