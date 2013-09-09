#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>

namespace library
{
	class vec4;
	class vec3;
	
	// light weight 2-vector class
	class vec2
	{
	public:
		typedef float vector_t;
		
		// fields
		vector_t x, y;
		
		// constructors
		vec2();
		vec2(vector_t);
		vec2(vector_t, vector_t);
		vec2(const vec2&);
		vec2(const vec3&);
		vec2(const vec4&);
		
		vector_t length() const;
		
		vec2& normalize();
		vec2  normalized() const;
		
	};
	
	// heavy-weight 3-vector class
	class vec3
	{
	public:
		typedef float vector_t;
		
		// fields
		vector_t x, y, z;
		
		// -= vec3 constructors =- //
		
		vec3();
		vec3(vector_t);
		vec3(vector_t, vector_t, vector_t);
		vec3(const vec3&);
		vec3(const vec4&); // creates vec3 from .xyz part
		
		// -= vec3 swizzles =- //
		
		vec2 xx() const; vec2 yy() const; vec2 zz() const;
		vec2 xy() const; vec2 yz() const; vec2 xz() const;
		
		// -= vec3 utilities =- //
		
		vector_t length() const;
		vec3& normalize();
		vec3  normalized() const;
		vector_t dot(const vec3&) const;
		vec3 frac() const;
		vec3 cross(const vec3&) const;
		vec3 reflect(const vec3&) const;
		vec3 mix(const vec3&, float) const;
		
		// rotate this vector by (angle) on (axis)
		vec3  rotateOnAxis(const vec3& axis, float angle) const;
		// transform this vector from pitch and yaw
		vec3& lookVector(const vec2& rot);
		vec2  toPitchYaw() const;
		
		// returns the smallest/biggest values of this(x, y, z)
		const vector_t min() const;
		const vector_t max() const;
		// returns the shortest/longest vectors between this and that
		const vec3& min(const vec3& that) const;
		const vec3& max(const vec3& that) const;
		
		// exponentiation
		vec3& pow(double e);
		vec3& pow(const vec3& v);
		
		// -= vec3 operators =- //
		
		// unary - (negate)
		const vec3 operator - () const;
		
		// arithmetic operators
		vec3& operator += (const vec3&);
		vec3& operator -= (const vec3&);
		vec3& operator *= (const vec3&);
		vec3& operator /= (const vec3&);
		
		vec3 operator + (const vec3&) const;
		vec3 operator - (const vec3&) const;
		vec3 operator * (const vec3&) const;
		vec3 operator / (const vec3&) const;
		
		vec3& operator +=(const vector_t);
		vec3& operator -=(const vector_t);
		vec3& operator *=(const vector_t);
		vec3& operator /=(const vector_t);
		
		vec3  operator + (const vector_t) const;
		vec3  operator - (const vector_t) const;
		vec3  operator * (const vector_t) const;
		vec3  operator / (const vector_t) const;
		
		// boolean equality operators
		bool operator == (const vec3&) const;
		bool operator != (const vec3&) const;
		
		// inline friends
		inline friend vec3 pow(const vec3& v1, double e)
		{
			return vec3(v1).pow(e);
		}
		inline friend vec3 pow(const vec3& v1, const vec3& v2)
		{
			return vec3(v1).pow(v2);
		}
		
	};
	
	// special homogenous 4-vector class
	class vec4 : public vec3
	{
	public:
		// homogenous coordinate
		vector_t w;
		
		// -= vec4 constructors =- //
		
		vec4();
		vec4(vector_t);
		vec4(vector_t, vector_t);
		vec4(vector_t, vector_t, vector_t, vector_t);
		vec4(const vec3&, vector_t);
		
		// -= vec4 swizzles =- //
		
		vec3 xyz() const;
		
		// -= vec4 operators =- //
		
		// unary - (negate)
		const vec4 operator - () const;
		
	};
	
	// vector language functions
	vec3::vector_t dot(const vec3& va, const vec3& vb);
	vec3 distance(const vec3& va, const vec3& vb);
	vec3 normalize(const vec3&);
	//void normalize(vec3&);
	vec3 cross(const vec3&, const vec3&);
	vec3 reflect(const vec3& I, const vec3& N);
	vec3 refract(const vec3& I, const vec3& N, const vec3::vector_t eta);
	
	// additional arithmetic operators
	template <typename T>
	vec3 operator + (const T t, const vec3& v)
	{
		return vec3(v.x + t, v.y + t, v.z + t);
	}
	template <class T>
	vec3 operator - (const T t, const vec3& v)
	{
		return vec3(v.x - t, v.y - t, v.z - t);
	}
	template <class T>
	vec3 operator * (const T t, const vec3& v)
	{
		return vec3(v.x * t, v.y * t, v.z * t);
	}
	template <class T>
	vec3 operator / (const T t, const vec3& v)
	{
		return vec3(v.x / t, v.y / t, v.z / t);
	}
	
	// log output functions
	class Log;
	Log& operator<< (Log& out, const vec2& v);
	Log& operator<< (Log& out, const vec3& v);
	Log& operator<< (Log& out, const vec4& v);
	
}

// cout output functions
std::ostream& operator<< (std::ostream& out, const library::vec2& v);
std::ostream& operator<< (std::ostream& out, const library::vec3& v);
std::ostream& operator<< (std::ostream& out, const library::vec4& v);

#endif
