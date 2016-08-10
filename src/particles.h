#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#include "vectors.h"
#include <vector>

class SKIER;
class CAMERA;

typedef struct particle
{
	vec3 position;
	vec3 velocity;
	float age;
	float size;
	bool enabled;
} Particle, PARTICLE;





class EMITTER
{
public:
	EMITTER(const int particles);
	virtual ~EMITTER();

	void init();
	void init(particle &p);
	void update(SKIER *skier, const float ft);
	void render(CAMERA * c);


private:
	particle* _particles;
	int _count;

	vec3 * _vertices;

	vec3 _position;		/* Position of Emitter */
	vec3 _direction;	/* Direction of Emitter */
};




#if 0
class PARTICLESYSTEM
{
public:
	static const unsigned int PARTICLE_COUNT;

	PARTICLESYSTEM();
	virtual ~PARTICLESYSTEM();


	bool addEmitter(EMITTER * const e);
	bool removeEmitter(EMITTER * const e);

	bool add(const PARTICLE &p);
	
	void update(const float ft);
	void render(CAMERA * c);

private:
	void init();

	std::vector<EMITTER*> _emitters;

	vec3 * _vertices;
	vec2 * _texCoords;
	int _particles;

};
#endif

#endif