/*
 * Name: Chris Snell      
 * Student ID: s3435406
 *
 * Class for handling a player
 */

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "vectors.h"
#include "utility.h"


class TERRAIN;





class SKIER
{
public:
	static const float ACCELERATION;
	static const float ACCELERATION_ANGULAR;
	static const float RESISTANCE;
	static const float RESISTANCE_ANGULAR;

	SKIER(vec3 startPos = vec3(0,0,0) );
	virtual ~SKIER();


	void forward(bool b);
	void backward(bool b);
	void left(bool b);
	void right(bool b);


	void update(double ft, TERRAIN * terrain);
	void render(bool lighting, bool axis);

	vec3 position();
	inline float rotation()	{return _rotation.y;}
	inline vec3 velocity() {return _velocity;}

	inline float radius() {return _radius;}

	inline float airTime()	{return _airTime;}
	inline int collisions()	{return _collisions;}


	void testCollision(const vec3 &p, const float r);


	inline bool hasContact() {return _contact;}

private:

	void create(float size);

	void rotate(float w);
	void move(float w);

	int _collisions;
	float _airTime;

	
	vec3 *_vertices;
	vec3 *_normals;
	int _faces;


	float _radius;
	vec3 _acceleration;
	vec3 _position;
	vec3 _velocity;

	vec3 _rotation;
	
	float _linAcceleration;
	float _angAcceleration;
	float _angVelocity;
	//float _angle;



	bool _forward;
	bool _backward;
	bool _left;
	bool _right;


	bool _contact;


	/* Debugging Lines */
	vec3 _lineNormal;
	vec3 _lineGravity;
	vec3 _lineGravityAngle;
	vec3 _lineGravityTravel;
};





#endif