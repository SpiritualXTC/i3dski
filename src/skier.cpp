/*
 * Name: Chris Snell      
 * Student ID: s3435406
 *
 */

#include "def.h"

#ifdef DEBUG
#include <stdio.h>
#endif

#include "skier.h"
#include "terrain.h"
#include "utility.h"


/* Setup Constants */
float const SKIER::ACCELERATION = 15.0f;	//10
float const SKIER::ACCELERATION_ANGULAR = 60000.0f * (float)PI/180.0f;
float const SKIER::RESISTANCE = 0.5f;//1.0f;
float const SKIER::RESISTANCE_ANGULAR = 192.0f;

/*
	PLAYER():
	Constructor
*/
SKIER::SKIER(vec3 startPos)
{
	_collisions = 0;
	_airTime = 0.0f;
	_contact = false;


	_radius = 1.0f;

	_vertices = NULL;
	_normals = NULL;
	_faces = 0;

	_acceleration = vec3();
	_position = startPos;
	_velocity = vec3();

	

	_rotation = vec3();
	//_angle = 0.0f;
	_angAcceleration = 0.0f;
	_linAcceleration = 0.0f;
	_angVelocity = 0.0f;

	_forward = false;
	_backward = false;
	_left = false;
	_right = false;



	/* Debug Lines */
	_lineNormal = vec3(0,1,0);
	_lineGravity = vec3();
	_lineGravityAngle = vec3();
	_lineGravityTravel = vec3();

	create(1.0f);
}


/*
	~PLAYER():
	Destructor
*/
SKIER::~SKIER()
{
#ifdef DEBUG
	printf("Player Destroyed\n");
#endif
	/* Free Memory */
	if (_normals != NULL)
		free( (void*)_normals );
	_normals = NULL;

	if (_vertices != NULL)
		free( (void*)_vertices );
	_vertices = NULL;
}


/*
	create():
*/
void SKIER::create(float size)
{
	float s = size * 0.5f;

	_faces = 6;

	/* Allocate Memory */
	_vertices = (vec3*)calloc(_faces * 4, sizeof(vec3));
	_normals = (vec3*)calloc(_faces * 4, sizeof(vec3));

	vec3* v = _vertices;
	vec3* n = _normals;

	/* Front +X */
	*(v++) = vec3( s,-s,-s);
	*(v++) = vec3( s, s,-s);
	*(v++) = vec3( s, s, s);
	*(v++) = vec3( s,-s, s);
	*(n++) = vec3(1, 0, 0);
	*(n++) = vec3(1, 0, 0);
	*(n++) = vec3(1, 0, 0);
	*(n++) = vec3(1, 0, 0);

	/* Back -X */
	*(v++) = vec3(-s,-s,-s);
	*(v++) = vec3(-s, s,-s);
	*(v++) = vec3(-s, s, s);
	*(v++) = vec3(-s,-s, s);
	*(n++) = vec3(-1, 0, 0);
	*(n++) = vec3(-1, 0, 0);
	*(n++) = vec3(-1, 0, 0);
	*(n++) = vec3(-1, 0, 0);

	/* Top +Y */
	*(v++) = vec3(-s, s,-s);
	*(v++) = vec3( s, s,-s);
	*(v++) = vec3( s, s, s);
	*(v++) = vec3(-s, s, s);
	*(n++) = vec3(0, 1, 0);
	*(n++) = vec3(0, 1, 0);
	*(n++) = vec3(0, 1, 0);
	*(n++) = vec3(0, 1, 0);

	/* Bottom -Y */
	*(v++) = vec3(-s,-s,-s);
	*(v++) = vec3( s,-s,-s);
	*(v++) = vec3( s,-s, s);
	*(v++) = vec3(-s,-s, s);
	*(n++) = vec3( 0,-1, 0);
	*(n++) = vec3( 0,-1, 0);
	*(n++) = vec3( 0,-1, 0);
	*(n++) = vec3( 0,-1, 0);

	/* Right +Z */
	*(v++) = vec3(-s,-s, s);
	*(v++) = vec3( s,-s, s);
	*(v++) = vec3( s, s, s);
	*(v++) = vec3(-s, s, s);
	*(n++) = vec3(0, 0, 1);
	*(n++) = vec3(0, 0, 1);
	*(n++) = vec3(0, 0, 1);
	*(n++) = vec3(0, 0, 1);

	/* Left -Z */
	*(v++) = vec3(-s,-s,-s);
	*(v++) = vec3( s,-s,-s);
	*(v++) = vec3( s, s,-s);
	*(v++) = vec3(-s, s,-s);
	*(n++) = vec3( 0, 0,-1);
	*(n++) = vec3( 0, 0,-1);
	*(n++) = vec3( 0, 0,-1);
	*(n++) = vec3( 0, 0,-1);
}



/*
	testCollision():

	Tests and Handles Collision Detection.
	...seriously bad implementation, but OK...!

	No Conservation of momentum is being handled,
	only reflective bouncing
*/
void SKIER::testCollision(const vec3 &p, const float r)
{
	vec3 d = _position - p;


	/* Too Far Away */
	if (d.length() > r + _radius)
		return;
	

	vec3 n = d / d.length();

	/* Heading Away */
	if ( n.dot(_velocity) > 0)
		return;



	/* Collision - moving slow enough to be at rest */
	if (_velocity.length() > 0.5f)
		_collisions++;

	/* Velocity Change */
	_velocity -= n * n.dot(_velocity) * 2;



	//printf("collision\n");

	return;
}




/*
	position():
	Get Player Position
*/
vec3 SKIER::position()
{
	return _position;
}


/*
	update():
	Update the player
*/
void SKIER::update(double ft, TERRAIN * terrain)
{
	/*
		Collision Terrain
		PSUEDO:

		
		Y < Hieght, Y = Height && Orientation Alignment
		
		Gravity in Direction of slope -9.8m/s/s * Slope ANGLE!
		Acceleration in direction of forward vector


		Direction DOT Velocity, may give a nice resistance value, for more forward resistance during a turn!
	*/

	
	/* Reset Accelerations */
	_acceleration = vec3();
	_angAcceleration = 0.0f;
	_linAcceleration = 0.0f;


	float height = 0.0f;
	float m, f, w;
	float angle = 0.0f;	/* Ramp Angle */

	vec3 a, g, n, d;

	a = vec3();
	g = vec3();
	n = vec3();

	/* Calculate Direction - this is only XZ. Should be X/Y/Z */
	d = vec3(cosf(_rotation.y), 0, -sinf(_rotation.y) );

	/* Update Rotation Position! */
	_position += _velocity * (float)ft;
	_rotation.y += _angVelocity * (float)ft;
	
	/* Update Input Forces */
	if (_left)
		rotate(1.0f);
	if (_right)
		rotate(-1.0f);
	if (_forward)
		move(1.0f);
	if (_backward)
		move(-1.0f);


	/* Get Values from Terrain */
	height = terrain->calcHeight(_position.x, _position.z);
	vec3 normal = terrain->calcNormal(_position.x, _position.z);
	vec3 slope = terrain->calcSlope(_position.x, _position.z);

	_contact = (_position.y < height+0.01f);




	m = 100.0f;
	w = m * TERRAIN::GRAVITY;

	if (_contact)
	{
		angle = -acosf(normal.y);
		//angle = normal.x < 0 ? acosf(normal.y) : -acosf(normal.y);


		f = w * sinf(angle);

		n = normal;


		/* Calculate Input Acceleration */
		a = d * _linAcceleration;



		g.x = f * cosf(angle) / m * slope.x;
		g.y = f * sinf(angle) / m;
		g.z = f * cosf(angle) / m * slope.z;

		
		_acceleration += a;
		_acceleration += g;
	}
	else
	{
		_acceleration += vec3(0,1,0) * TERRAIN::GRAVITY;
		//Calculate delta a rotation across z axis

		_airTime += ft;
	}


	_lineGravityTravel = g;
	_lineGravityAngle = slope;
	_lineGravity = vec3(0,1,0) * TERRAIN::GRAVITY;
	_lineNormal	= n;

	/* Apply Resistances */
	_acceleration += -_velocity * RESISTANCE;
	_angAcceleration += -_angVelocity * RESISTANCE_ANGULAR;

	/* Update Velocities */
	_velocity += _acceleration * (float)ft;	
	_angVelocity += _angAcceleration * (float)ft;
	
	/* Align Position With Terrain */
	if (_position.y < height)
	{
		_position.y = height;

		_rotation.x = -asinf(n.x);
		_rotation.z =  asinf(n.z);

		_velocity -= n * n.dot(_velocity);
	}
}


/*
	render():
	Render the Player
*/
void SKIER::render(bool lighting, bool axis)
{
	GLfloat black[] = {0,0,0,0};
	GLfloat white[] = {1,1,1,1};
	color dif = color(1.0f,0.5f,0.0f,1.0f);
	color amb = dif * 0.25f;
	GLfloat shiny = 10.0f;








	if (lighting)
	{
		glEnable(GL_LIGHTING);

		/* Set Material */
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (float*)&amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (float*)&dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
	}
	else
		glColor3fv((float*)&dif);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(_position.x, _position.y, _position.z);

	/* Rotation */
	glRotatef((float)DEGREES(_rotation.x), 0.0f, 0.0f, 1.0f);
	glRotatef((float)DEGREES(_rotation.z), 1.0f, 0.0f, 0.0f);

	glRotatef((float)DEGREES(_rotation.y), 0.0f, 1.0f, 0.0f);

	/* Draw Cube */

	/* Enable Arrays */
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	
	/* Set Arrays */
	glNormalPointer(GL_FLOAT, 0, _normals);
	glVertexPointer(3, GL_FLOAT, 0, _vertices);

	/* Draw Vertices */
	glDrawArrays(GL_QUADS, 0, _faces * 4);

	/* Disable Arrays */
	glDisableClientState( GL_NORMAL_ARRAY);
	glDisableClientState( GL_VERTEX_ARRAY);


	if (lighting)
		glDisable(GL_LIGHTING);

	/* Render Axis */
	if (axis)
	{
		glDisable(GL_DEPTH_TEST);
		renderAxis(5.0f);
		glEnable(GL_DEPTH_TEST);
	}




	glPopMatrix();










	


	/* Render Terrain Normal @ Position */
#ifdef DEBUG_PLAYER
	if (axis)
	{
		glDisable(GL_DEPTH_TEST);
		
		/* Render Terrain Normal @ Position */
		renderLine(_position, _position + _lineNormal * 40.0f, color(1,1,1));



		renderLine(_position, _position + _acceleration, color(0.0f,0.5f,0.5f));
		renderLine(_position, _position + _velocity, color(0.5f,0.0f,0.5f));


		/* Gravity Lines */
		renderLine(_position, _position + _lineGravity, color(0.2f,0.4f,0.8f));
		renderLine(_position, _position + _lineGravityAngle, color(0.2f,0.8f,0.4f));
		renderLine(_position + _lineGravity, _position + _lineGravityAngle, color(0.2f,0.8f,0.4f));


		renderLine(_position, _position + _lineGravityTravel, color(1,1,0));



		/* Gravity in OPPOSITE Direction */
	//	renderLine(_position, _position - _gravityAngle, color(0,1,1));



		glEnable(GL_DEPTH_TEST);

		/* Draw a Plane at the position of the player */
		//renderPlane(_normal, _position);
#endif
	}

	
	
}





/*
	rotate():
	Accelerate the angular rotation
*/
void SKIER::rotate(float w)
{
	_angAcceleration += ACCELERATION_ANGULAR * w;
}

/*
	move():
	Accelerate in the direction of the player
*/
void SKIER::move(float w)
{
	_linAcceleration += ACCELERATION * w;
}


/*
	forward():
	Set the Forwards Movement Flag
*/
void SKIER::forward(bool b)
{
	_forward = b;
}

/*
	backward():
	Set the Backwards Movement Flag
*/
void SKIER::backward(bool b)
{
	_backward = b;
}

/*
	left():
	Set the Turn Left Flag
*/
void SKIER::left(bool b)
{
	_left = b;
}


/*
	right():
	Set the Turn Right Flag
*/
void SKIER::right(bool b)
{
	_right = b;
}


