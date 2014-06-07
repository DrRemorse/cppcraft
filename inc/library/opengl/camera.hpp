#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <library/math/matrix.hpp>
#include <library/math/frustum.hpp>

namespace library
{
	class WindowClass;
	
	class Camera
	{
	protected:
		float FOV;         //! \brief field of view (angle in degrees)
		float znear, zfar; //! \brief znear and zfar planes
		
		// viewing frustum representation
		library::Frustum frustum;
		
		// camera matrices
		library::mat4 matproj;     //! \brief projection matrix (P)
		library::mat4 matrot;      //! \brief camera rotation matrix (R)
		library::mat4 matview;     //! \brief modelview matrix (MV)
		library::mat4 matviewproj; //! \brief modelview-projection matrix (MVP)
		// used to reconstruct viewpos from windowspace depth
		library::vec2 nearPlaneHalfSize;
		
	public:
		Camera() {}
		Camera(float FOV, float aspect, float znear, float zfar);
		
		void calculateFrustum();
		const library::Frustum& getFrustum() const;
		
		//! \brief returns the perspective field of view in degrees
		inline float getFOV()   const { return this->FOV; }
		//! \brief returns the distance from eye to znear plane
		inline float getZNear() const { return this->znear; }
		//! \brief returns the distance from eye to zfar plane
		inline float getZFar()  const { return this->zfar; }
		
		//! \brief set perspective projection matrix
		void setProjection(float FOV, float aspect, float znear, float zfar);
		//! \brief set rotation and translation (unstacked)
		void setRotation   (float rx, float ry, float rz);
		void setTranslation(float tx, float ty, float tz);
		
		//! \brief directly set rotation matrix
		void setRotationMatrix(const mat4& rotation_matrix);
		//! \brief directly set modelview matrix
		void setViewMatrix(const mat4& view_matrix);
		
		const library::mat4& getProjection() const
		{
			return matproj;
		}
		const library::mat4& getViewMatrix() const
		{
			return matview;
		}
		const library::mat4& getRotationMatrix() const
		{
			return matrot;
		}
		const library::mat4& getMVP() const
		{
			return matviewproj;
		}
		
		const library::vec2& getNearPlaneHalfSize() const
		{
			return nearPlaneHalfSize;
		}
	};
	
}

#endif
