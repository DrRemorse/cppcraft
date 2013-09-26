/**
 * 4x4 Matrix implementation
 * 
**/
#include "matrix.hpp"
#include <cmath>

namespace library
{
	Matrix::Matrix() {}
	
	Matrix::Matrix(const Matrix& matrix)
	{
		for (int i = 0; i < ELEMENTS; i++)
			m[i] = matrix[i];
	}
	Matrix::Matrix(matrix_t matrix[ELEMENTS])
	{
		for (int i = 0; i < ELEMENTS; i++)
			m[i] = matrix[i];
		
	}
	
	// scale matrices
	Matrix::Matrix(matrix_t sc)
	{
		m[0] = sc;  m[4] = 0.0; m[ 8] = 0.0; m[12] = 0.0;
		m[1] = 0.0; m[5] = sc;  m[ 9] = 0.0; m[13] = 0.0;
		m[2] = 0.0; m[6] = 0.0; m[10] = sc;  m[14] = 0.0;
		m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
	}
	Matrix::Matrix(matrix_t sx, matrix_t sy, matrix_t sz)
	{
		m[0] = sx;  m[4] = 0.0; m[ 8] = 0.0; m[12] = 0.0;
		m[1] = 0.0; m[5] = sy;  m[ 9] = 0.0; m[13] = 0.0;
		m[2] = 0.0; m[6] = 0.0; m[10] = sz;  m[14] = 0.0;
		m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
	}
	
	Matrix::matrix_t* Matrix::data()
	{
		return this->m;
	}
	
	Matrix& Matrix::identity()
	{
		m[0] = 1.0; m[4] = 0.0; m[ 8] = 0.0; m[12] = 0.0;
		m[1] = 0.0; m[5] = 1.0; m[ 9] = 0.0; m[13] = 0.0;
		m[2] = 0.0; m[6] = 0.0; m[10] = 1.0; m[14] = 0.0;
		m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
		return *this;
	}
	
	Matrix& Matrix::bias()
	{
		m[0] = 0.5; m[4] = 0.0; m[8 ] = 0.0; m[12] = 0.5;
		m[1] = 0.0; m[5] = 0.5; m[9 ] = 0.0; m[13] = 0.5;
		m[2] = 0.0; m[6] = 0.0; m[10] = 0.5; m[14] = 0.5;
		m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
		return *this;
	}
	
	vec4 Matrix::vright() const
	{
		return vec4(m[0], m[1], m[2], m[3]);
	}
	vec4 Matrix::vup() const
	{
		return vec4(m[4], m[5], m[6], m[7]);
	}
	vec4 Matrix::vforward() const
	{
		return vec4(m[8], m[9], m[10], m[11]);
	}
	vec4 Matrix::vtranslate() const
	{
		return vec4(m[12], m[13], m[14], m[15]);
	}
	
	// transform 3-vector (w = 1.0)
	vec3 Matrix::operator* (const vec3& v) const
	{
		return vec3(
			v.x * m[0] + v.y * m[4] + v.z * m[ 8] + m[12],
			v.x * m[1] + v.y * m[5] + v.z * m[ 9] + m[13],
			v.x * m[2] + v.y * m[6] + v.z * m[10] + m[14]
		);
	}
	// transform 4-vector
	vec4 Matrix::operator* (const vec4& v) const
	{
		return vec4(
			v.x * m[0] + v.y * m[4] + v.z * m[ 8] + v.w * m[12],
			v.x * m[1] + v.y * m[5] + v.z * m[ 9] + v.w * m[13],
			v.x * m[2] + v.y * m[6] + v.z * m[10] + v.w * m[14],
			v.x * m[3] + v.y * m[7] + v.z * m[11] + v.w * m[15]
		);
	}
	
	Matrix Matrix::operator * (const Matrix& matrix) const
	{
		return Matrix(*this) *= matrix;
	}
	
	Matrix& Matrix::operator *= (const Matrix& matrix)
	{
		Matrix temp(*this);
		
		m[0] = temp.m[0 ] * matrix.m[0 ]
			 + temp.m[4 ] * matrix.m[1 ]
			 + temp.m[8 ] * matrix.m[2 ]
			 + temp.m[12] * matrix.m[3 ];
		
		m[1] = temp.m[1 ] * matrix.m[0 ]
			 + temp.m[5 ] * matrix.m[1 ]
			 + temp.m[9 ] * matrix.m[2 ]
			 + temp.m[13] * matrix.m[3 ];
		
		m[2] = temp.m[2 ] * matrix.m[0 ]
			 + temp.m[6 ] * matrix.m[1 ]
			 + temp.m[10] * matrix.m[2 ]
			 + temp.m[14] * matrix.m[3 ];
		
		m[3] = temp.m[3 ] * matrix.m[0 ]
			 + temp.m[7 ] * matrix.m[1 ]
			 + temp.m[11] * matrix.m[2 ]
			 + temp.m[15] * matrix.m[3 ];
		
		m[4] = temp.m[0 ] * matrix.m[4 ]
			 + temp.m[4 ] * matrix.m[5 ]
			 + temp.m[8 ] * matrix.m[6 ]
			 + temp.m[12] * matrix.m[7 ];
		
		m[5] = temp.m[1 ] * matrix.m[4 ]
			 + temp.m[5 ] * matrix.m[5 ]
			 + temp.m[9 ] * matrix.m[6 ]
			 + temp.m[13] * matrix.m[7 ];
		
		m[6] = temp.m[2 ] * matrix.m[4 ]
			 + temp.m[6 ] * matrix.m[5 ]
			 + temp.m[10] * matrix.m[6 ]
			 + temp.m[14] * matrix.m[7 ];
		
		m[7] = temp.m[3 ] * matrix.m[4 ]
			 + temp.m[7 ] * matrix.m[5 ]
			 + temp.m[11] * matrix.m[6 ]
			 + temp.m[15] * matrix.m[7 ];
		
		m[8] = temp.m[0 ] * matrix.m[8 ]
			 + temp.m[4 ] * matrix.m[9 ]
			 + temp.m[8 ] * matrix.m[10]
			 + temp.m[12] * matrix.m[11];
		
		m[9] = temp.m[1 ] * matrix.m[8 ]
			 + temp.m[5 ] * matrix.m[9 ]
			 + temp.m[9 ] * matrix.m[10]
			 + temp.m[13] * matrix.m[11];
		
		m[10]= temp.m[2 ] * matrix.m[8 ]
			 + temp.m[6 ] * matrix.m[9 ]
			 + temp.m[10] * matrix.m[10]
			 + temp.m[14] * matrix.m[11];
		
		m[11]= temp.m[3 ] * matrix.m[8 ]
			 + temp.m[7 ] * matrix.m[9 ]
			 + temp.m[11] * matrix.m[10]
			 + temp.m[15] * matrix.m[11];

		m[12]= temp.m[0 ] * matrix.m[12]
			 + temp.m[4 ] * matrix.m[13]
			 + temp.m[8 ] * matrix.m[14]
			 + temp.m[12] * matrix.m[15];

		m[13]= temp.m[1 ] * matrix.m[12]
			 + temp.m[5 ] * matrix.m[13]
			 + temp.m[9 ] * matrix.m[14]
			 + temp.m[13] * matrix.m[15];

		m[14]= temp.m[2 ] * matrix.m[12]
			 + temp.m[6 ] * matrix.m[13]
			 + temp.m[10] * matrix.m[14]
			 + temp.m[14] * matrix.m[15];

		m[15]= temp.m[3 ] * matrix.m[12]
			 + temp.m[7 ] * matrix.m[13]
			 + temp.m[11] * matrix.m[14]
			 + temp.m[15] * matrix.m[15];
		
		return *this;
	}
	
	void Matrix::translate(matrix_t x, matrix_t y, matrix_t z)
	{
		// set translation vector (x, y, z, 1)
		m[12] = x;
		m[13] = y;
		m[14] = z;
		m[15] = 1.0;
	}
	
	void Matrix::translated(matrix_t x, matrix_t y, matrix_t z)
	{
		// translate with delta +(x, y, z)
		m[12] +=  m[0 ] * x + m[4 ] * y + m[8 ] * z;
		m[13] +=  m[1 ] * x + m[5 ] * y + m[9 ] * z;
		m[14] +=  m[2 ] * x + m[6 ] * y + m[10] * z;
		//m[15] +=  m[3 ] * x + m[7 ] * y + m[11] * z;
	}
	
	void Matrix::translate_xy(matrix_t x, matrix_t y)
	{
		// translate with delta +(x, y)
		m[12] +=  m[0 ] * x + m[4] * y;
		m[13] +=  m[1 ] * x + m[5] * y;
		m[14] +=  m[2 ] * x + m[6] * y;
		//m[15] +=  m[3 ] * x + m[7] * y;
	}
	
	void Matrix::translate_xz(matrix_t x, matrix_t z)
	{
		// translate with delta +(x, y)
		m[12] +=  m[0 ] * x + m[ 8] * z;
		m[13] +=  m[1 ] * x + m[ 9] * z;
		m[14] +=  m[2 ] * x + m[10] * z;
		//m[15] +=  m[3 ] * x + m[7 ] * y;
	}
	
	void Matrix::scale(matrix_t sc)
	{
		m[0] = sc;  m[4] = 0.0; m[ 8] = 0.0; m[12] = 0.0;
		m[1] = 0.0; m[5] = sc;  m[ 9] = 0.0; m[13] = 0.0;
		m[2] = 0.0; m[6] = 0.0; m[10] = sc;  m[14] = 0.0;
		m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
	}
	
	void Matrix::scale(matrix_t sx, matrix_t sy, matrix_t sz)
	{
		m[0] = sx;  m[4] = 0.0; m[ 8] = 0.0; m[12] = 0.0;
		m[1] = 0.0; m[5] = sy;  m[ 9] = 0.0; m[13] = 0.0;
		m[2] = 0.0; m[6] = 0.0; m[10] = sz;  m[14] = 0.0;
		m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
	}
	
	void Matrix::rotateZYX(matrix_t x, matrix_t y, matrix_t z)
	{
		//////////////////////////////////////////////////////////////////////////////
		// convert Euler angles(x,y,z) to axes(left, up, forward)					//
		// Each column of the rotation matrix represents left, up and forward axis. //
		// The order of rotation is Roll->Yaw->Pitch (Rx*Ry*Rz)
		// Rx: rotation about X-axis, pitch
		// Ry: rotation about Y-axis, yaw(heading)
		// Rz: rotation about Z-axis, roll
		//    Rx           Ry          Rz
		// |1  0   0| | Cy  0 Sy| |Cz -Sz 0|   | CyCz        -CySz         Sy  |	//
		// |0 Cx -Sx|*|  0  1  0|*|Sz  Cz 0| = | SxSyCz+CxSz -SxSySz+CxCz -SxCy|	//
		// |0 Sx  Cx| |-Sy  0 Cy| | 0   0 1|   |-CxSyCz+SxSz  CxSySz+SxCz  CxCy|	//
		//////////////////////////////////////////////////////////////////////////////
		
		// rotation angle about X-axis (pitch)
		matrix_t sx = sin(x);
		matrix_t cx = cos(x);
		
		// rotation angle about Y-axis (yaw)
		matrix_t sy = sin(y);
		matrix_t cy = cos(y);
		
		if (z != 0.0)
		{
			// rotation angle about Z-axis (roll)
			matrix_t sz = sin(z);
			matrix_t cz = cos(z);
			
			// left vector
			m[0] =  cy * cz;
			m[1] =  sx * sy * cz + cx * sz;
			m[2] = -cx * sy * cz + sx * sz;
			m[3] = 0.0; // w1
			
			// up vector
			m[4] = -cy * sz;
			m[5] = -sx * sy * sz + cx * cz;
			m[6] =  cx * sy * sz + sx * cz;
			m[7] = 0.0; // w2
		}
		else
		{
			// sz = 0.0 (sin(0) = 0)
			// cz = 1.0 (cos(0) = 1)
			
			// left vector
			m[0] =  cy;
			m[1] =  sx * sy;
			m[2] = -cx * sy;
			m[3] = 0.0; // w1
			
			// up vector
			m[4] = 0.0;
			m[5] = cx;
			m[6] = sx;
			m[7] = 0.0; // w2
		}
		
		// forward vector
		m[ 8] =  sy;
		m[ 9] = -sx * cy;
		m[10] =  cx * cy;
		m[11] = 0.0; // w3
		
		m[12] = 0.0; // tx
		m[13] = 0.0; // ty
		m[14] = 0.0; // tz
		m[15] = 1.0; // w4
	}
	
	// specialized 4x4 transpose
	// returns this matrix transposed
	Matrix& Matrix::transpose()
	{
		matrix_t temp;
		
		// swap y, x
		temp = m[1]; m[1] = m[4]; m[4] = temp;
		temp = m[2]; m[2] = m[8]; m[8] = temp;
		temp = m[6]; m[6] = m[9]; m[9] = temp;
		
		// invert (tx, ty, tz)
	  	m[12] *= -1.0;
	  	m[13] *= -1.0;
	  	m[14] *= -1.0;
		
		return *this;
	}
	// returns new transposed matrix
	Matrix Matrix::transposed() const
	{
		Matrix mat(*this);
		return mat.transpose();
	}
	
	// batch transform vertices from memory location
	void Matrix::batch(void* first, int stride, int count)
	{
		for (int i = 0; i < count; i++)
		{
			float* v = (float*) ((char*)first + i * stride);
			v[0] = v[0] * m[0] + v[1] * m[4] + v[2] * m[ 8] + m[12];
			v[1] = v[0] * m[1] + v[1] * m[5] + v[2] * m[ 9] + m[13];
			v[2] = v[0] * m[2] + v[1] * m[6] + v[2] * m[10] + m[14];
		}
	}
	
	// returns translation (tx, ty, tz) from a view matrix (rotation + translation)
	vec3 Matrix::transVector() const
	{
		return vec3(0); //m[12], m[13], m[14]);
		/*
		return vec3(
			v.x * m[0] + v.y * m[4] + v.z * m[ 8] + m[12],
			v.x * m[1] + v.y * m[5] + v.z * m[ 9] + m[13],
			v.x * m[2] + v.y * m[6] + v.z * m[10] + m[14]
		);*/
	}
	
	// returns rotation (rx, ry, rz) from a view matrix (rotation + translation)
	vec3 Matrix::lookVector() const
	{
		return vec3(m[8], m[9], -m[10]);
	}
	
	Matrix Matrix::rotation() const
	{
		Matrix rot(*this);
		rot.translate(0, 0, 0);
		return rot;
	}
	
	// screen-coordinate-based orthographic projection matrix
	void Matrix::orthoScreen(matrix_t width, matrix_t height, matrix_t znear, matrix_t zfar)
	{
		matrix_t depth = zfar - znear;
		
		m[0] = 2/width;  m[4] = 0;         m[ 8] = 0;         m[12] = -1.0;
		m[1] = 0;        m[5] = 2/height;  m[ 9] = 0;         m[13] = -1.0;
		m[2] = 0;        m[6] = 0;         m[10] = -2/depth;  m[14] = -(zfar+znear) / depth;
		m[3] = 0;        m[7] = 0;         m[11] = 0;         m[15] =  1.0;
	}
	
	// orthographic projection matrix
	void Matrix::ortho(matrix_t width, matrix_t height, matrix_t znear, matrix_t zfar)
	{
		matrix_t ndcw =  width;  // 
		matrix_t ndch = -height; // inverting Y-axis
		matrix_t depth = zfar - znear;
		
		m[0] = 2/ndcw;  m[4] = 0;       m[ 8] = 0;         m[12] = -1.0;
		m[1] = 0;       m[5] = 2/ndch;  m[ 9] = 0;         m[13] =  1.0;
		m[2] = 0;       m[6] = 0;       m[10] = -2/depth;  m[14] = -(zfar+znear) / depth;
		m[3] = 0;       m[7] = 0;       m[11] = 0;         m[15] =  1.0;
	}
	
	// FOV perspective matrix (frustum)
	void Matrix::perspective(matrix_t fov, matrix_t aspect, matrix_t znear, matrix_t zfar)
	{
		const matrix_t pio360 = 4.0 * atan(1.0) / 360.0;
		
		matrix_t h = 1.0 / tan(fov * pio360);
		matrix_t negd = znear - zfar;
		
		m[ 0] = h / aspect; m[ 1] = 0; m[ 2] = 0;                       m[ 3] =  0;
		m[ 4] = 0;          m[ 5] = h; m[ 6] = 0;                       m[ 7] =  0;
		m[ 8] = 0;          m[ 9] = 0; m[10] = (zfar + znear) / negd;   m[11] = -1;
		m[12] = 0;          m[13] = 0; m[14] = 2 * znear * zfar / negd; m[15] =  0;
	}
	
	// additional operators
	vec4 operator * (const vec4& v, const Matrix& m)
	{
		return m.transposed() * v;
	}
	
	// additional functions
	Matrix transpose(const Matrix& m)
	{
		return m.transposed();
	}
	
}
