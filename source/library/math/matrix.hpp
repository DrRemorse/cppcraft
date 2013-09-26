/**
 * 4x4 Matrix class
 * 
 * Creating identity matrix in 2 ways:
 * 1. Matrix mat1(1.0);
 * 2. Matrix mat2;
 *    mat2.identity();
 * 
 * Creating scaling matrices:
 * 1. Matrix mat1(scaleXYZ);
 * 2. Matrix mat2(scaleX, scaleY, scaleZ);
 * 3. Matrix mat3;
 *    mat3.scale(scaleX, scaleY, scaleZ);
 * 
 * Transforming a vector:
 *    Matrix mat1(1.0);
 *    vec4 transformedVector = mat1 * vec4(1.0);
 * 
 * 
 * 
**/
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"

namespace library
{
	class Matrix
	{
	public:
		// datatype must be 32bit float (or the matrix will be useless)
		typedef float matrix_t;
		
		Matrix();
		Matrix(const Matrix&);
		Matrix(matrix_t[]);
		// scaling matrix constructors
		Matrix(matrix_t scale);
		Matrix(matrix_t sx, matrix_t sy, matrix_t sz);
		
		matrix_t* data();
		
		vec4 vright() const;	 // right-vector
		vec4 vup() const;		 // up-vector
		vec4 vforward() const;   // forward-vector
		vec4 vtranslate() const; // translation-vector
		
		// ye olde identity matrix
		Matrix& identity();
		// normalized device coordinates matrix
		Matrix& bias();
		
		inline const matrix_t& operator[] (int i) const
		{
			return m[i];
		}
		
		vec3    operator * (const vec3&) const;
		vec4    operator * (const vec4&) const;
		Matrix  operator * (const Matrix&) const;
		Matrix& operator *=(const Matrix&);
		
		// permanent transpose
		Matrix& transpose();
		// new matrix from transpose
		Matrix transposed() const;
		
		void scale(matrix_t scale);
		void scale(matrix_t x, matrix_t y, matrix_t z);
		
		void translate(matrix_t x, matrix_t y, matrix_t z);
		void translated(matrix_t x, matrix_t y, matrix_t z);
		void translate_xy(matrix_t x, matrix_t y);
		void translate_xz(matrix_t x, matrix_t z);
		
		void rotateZYX(matrix_t x, matrix_t y, matrix_t z);
		
		void ortho(matrix_t width, matrix_t height, matrix_t znear, matrix_t zfar);
		void orthoScreen(matrix_t width, matrix_t height, matrix_t znear, matrix_t zfar);
		void perspective(matrix_t fov, matrix_t aspect, matrix_t znear, matrix_t zfar);
		
		void batch(void* first, int stride, int count);
		
		// returns the transformed vector from this_matrix * vec4(0, 0, 0, 1)
		// essentially returning translation and hom.coord: (tx, ty, tz, w)
		vec3 transVector() const;
		vec3 lookVector() const;
		
		Matrix rotation() const;
		
		static const int AXES     = 4;
		static const int ELEMENTS = AXES * AXES;
		
	private:
		// data
		matrix_t m[ELEMENTS];
	};
	
	// additional operators
	vec4 operator* (const vec4&, const Matrix&); // transpose multiply
	
	// additional functions
	Matrix transpose(const Matrix&);
	
}

#endif
