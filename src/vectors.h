/*
 * Name: Chris Snell      
 * Student ID: s3435406
 *
 * Common datatypes
 */

#ifndef _VECTORS_H_
#define _VECTORS_H_


#define GENERICS
#undef GENERICS




#define EPSILON 0.0000001


/*
	2D int datatype.
*/
typedef struct point2
{
	point2();
	point2(int x, int y);

	int x;
	int y;
} Point2;








/*
	2D float datatype
*/
typedef struct vec2
{
	float x;
	float y;

	vec2();
	vec2(float fx, float fy);
} Vec2;



/*
	3D float datatype
*/
#ifndef GENERICS
typedef struct vec3
{
	float x;
	float y;
	float z;

	vec3();
	vec3(float fx, float fy, float fz);

	vec3& normalise();
	float length();
	vec3& length(float len);
	float dot(const vec3 &v);

	vec3& scale(float n);
	vec3& scale(float nx, float ny, float nz);

	bool operator==(const vec3& v);
	bool operator!=(const vec3& v);

	vec3 operator-(void);

	vec3& operator+=(const vec3 &v);
	vec3& operator-=(const vec3 &v);
	vec3& operator*=(const float f);
	vec3& operator/=(const float f);

	vec3 operator*(const float f);
	vec3 operator/(const float f);

	vec3 operator-(const vec3 &v);
	vec3 operator+(const vec3 &v);
	
	vec3 operator*(const vec3 &v);

} Vec3;
#endif



/*
	4D float datatype
*/
typedef struct vec4
{
	float x;
	float y;
	float z;
	float w;

	vec4();
	vec4(float fx, float fy, float fz);
	vec4(float fx, float fy, float fz, float fw);

} Vec4, VECTOR4;


/*
	Plane
*/
typedef struct plane
{
	float a;
	float b;
	float c;
	float d;

	plane();
	plane(float fx, float fy, float fz);
	plane(const vec3 &n);
	plane(const vec3 &n, const vec3 &p);

	bool intersect(vec3& out, vec3 &p0, vec3 &p1);

} Plane;




/*
	2D Float Region
*/
typedef struct region2
{
	vec2 minimum;
	vec2 maximum;

	inline float width()	{return maximum.x - minimum.x;}
	inline float height()	{return maximum.y - minimum.y;}

	region2();
	region2(float minX, float minY, float maxX, float maxY); 

} Region2;







#ifdef GENERICS
template<typename T>
struct v3
{
	T x;
	T y;
	T z;

	v3();
	v3(T fx, T fy, T fz);

	
	v3<T>& normalise();

	
	T length();

	
	T dot(const v3<T> &v);


	bool operator==(const v3<T> &v);
	
	v3<T> operator-(void);
	
	v3<T>& operator+=(const v3<T>& v);
	v3<T>& operator-=(const v3<T>& v);

	v3<T>& operator*=(const T& f);
	v3<T>& operator/=(const T& f);

	
	
	v3<T> operator*(const T& f);

	v3<T> operator-(const v3<T>& v);
	v3<T> operator+(const v3<T>& v);
	
	v3<T> operator*(const v3<T>& v);
	
} ;

typedef struct v3<float> Vec3;
typedef struct v3<float> vec3;
#endif

#endif
