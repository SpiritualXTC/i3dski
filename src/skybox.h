#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "def.h"
#include "vectors.h"

#define SKYBOX_PLANES 6
#define SKYBOX_VERTS 4

enum Skybox
{
	SKYBOX_X_POSITIVE,
	SKYBOX_Y_POSITIVE,
	SKYBOX_Z_POSITIVE,

	SKYBOX_X_NEGATIVE,
	SKYBOX_Y_NEGATIVE,
	SKYBOX_Z_NEGATIVE
};

class CAMERA;



typedef struct skybox_plane
{
	char texFn[80];
	vec3 verts[SKYBOX_VERTS];
	vec2 tex[SKYBOX_VERTS];

	unsigned int texture;
} SkyboxPlane;



class SKYBOX
{
public:
	SKYBOX();
	virtual ~SKYBOX();


	void render( CAMERA * camera);

private:

	void generate(float size);


//	unsigned int _texture;

/*
	unsigned int _faces;
	vec3 *_vertices;
	vec2 *_textures;
*/
	SkyboxPlane _planes[SKYBOX_PLANES];
};

#endif

