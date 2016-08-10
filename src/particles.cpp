
#include "particles.h"
#include "def.h"
#include "utility.h"
#include "terrain.h"

#include "camera.h"
#include "skier.h"


EMITTER::EMITTER(const int particles)
{
	_particles = NULL;
	_count = particles;
	_vertices = NULL;

	init();
}

EMITTER::~EMITTER()
{
	if (_vertices)
		free(_vertices);
	_vertices = NULL;

	if (_particles != NULL)
		free(_particles);
	_particles = NULL;
}



/* Initialise */
void EMITTER::init()
{
	_particles = (PARTICLE*)calloc(_count, sizeof(PARTICLE));

	_vertices = (vec3*)calloc(_count, sizeof(vec3));

	for (int i=0; i<_count; i++)
	{
		/* Init Particle */
		init(_particles[i]);
	}
}



/* Initialise a Particle */
void EMITTER::init(particle &p)
{
	p.position	= _position + vec3();
	p.size		= fRandom(0.125f, 0.25f);

	float v		= fRandom(4, 5);

	p.velocity	= vec3(v * _direction.x, fRandom(4,5), v * _direction.z);
	p.age		= fRandom(1);

	p.enabled	= true;
}



/* Update all the Particles */
void EMITTER::update(SKIER *skier, const float ft)
{
	PARTICLE * p = _particles;
	vec3 * v = _vertices;

	for (int i=0; i<_count; ++i, ++p)
	{
		p->position	+= p->velocity * ft;
		p->velocity.y += TERRAIN::GRAVITY * ft; 

		p->age -= ft;

		if ( p->age < 0.0f)
		{
			if (skier->hasContact())
				init(*p);
			else
				p->enabled = false;
		}
		else
		{
			if (p->enabled)
				(*v++) = vec3(-p->size, p->size, 0.0f) + p->position;
		}
	}

	/* Update Emitter Data */
	_position	= skier->position();
	_direction	= -(skier->velocity() / skier->velocity().length());
}



/* Render(): */
void EMITTER::render(CAMERA * c)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

//	glRotatef(-c->rotateTheta(), 0, 1, 0);
//	glRotatef(-c->rotatePhi(), 1, 0, 0);
	
	glColor3f(1, 1, 1);

	glEnableClientState( GL_VERTEX_ARRAY );
//	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer(3, GL_FLOAT, 0, _vertices);
//	glDrawArrays(GL_QUADS, 0, _count * 4);

	glDrawArrays(GL_POINTS, 0, _count);

//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}


