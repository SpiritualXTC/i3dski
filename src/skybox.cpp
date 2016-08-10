#include "skybox.h"

#include <stdio.h>


#include "camera.h"
#include "texture.h"



SKYBOX::SKYBOX()
{
	for (int i=0; i<SKYBOX_PLANES; i++)
	{
		_planes[i].texture = 0;
	}

	generate(5000);
}


SKYBOX::~SKYBOX()
{
	for (int i=0; i<SKYBOX_PLANES; i++)
	{
		if (_planes[i].texture)
			deleteTexture(_planes[i].texture);
		_planes[i].texture = 0;
	}
}


void SKYBOX::generate(float size)
{
	float s = size * 0.5f;

	char *fn[SKYBOX_PLANES] = 
	{
		"skybox_xpos.png",
		"skybox_ypos.png",
		"skybox_zpos.png",
		"skybox_xneg.png",
		"skybox_yneg.png",
		"skybox_zneg.png"
	};

	vec2 tex[SKYBOX_VERTS] = 
	{
		vec2(0, 1), vec2(1, 1), vec2(1, 0), vec2(0, 0)
	};



	vec3 xpos[SKYBOX_VERTS] = 
	{
		vec3( s, s,-s), vec3( s, s, s), vec3( s,-s, s), vec3( s,-s,-s)
	};

	vec3 ypos[SKYBOX_VERTS] = 
	{
		vec3(-s, s,-s), vec3(-s, s, s), vec3( s, s, s), vec3( s, s,-s)
	};

	vec3 zpos[SKYBOX_VERTS] = 
	{
		 vec3(-s, s, s), vec3(-s,-s, s), vec3( s,-s, s), vec3( s, s, s)
	};


	vec3 xneg[SKYBOX_VERTS] = 
	{
		vec3(-s,-s,-s), vec3(-s,-s, s), vec3(-s, s, s), vec3(-s, s,-s)
	};

	vec3 yneg[SKYBOX_VERTS] = 
	{
		vec3( s,-s,-s), vec3( s,-s, s), vec3(-s,-s, s), vec3(-s,-s,-s)
	};

	vec3 zneg[SKYBOX_VERTS] = 
	{
		 vec3(-s,-s,-s), vec3(-s, s,-s), vec3( s, s,-s), vec3( s,-s,-s)
	};


	/* Copy Coordinates */
	for (int i=0; i<SKYBOX_VERTS; i++)
	{
		_planes[SKYBOX_X_POSITIVE].verts[i] = xpos[i];
		_planes[SKYBOX_Y_POSITIVE].verts[i] = ypos[i];
		_planes[SKYBOX_Z_POSITIVE].verts[i] = zpos[i];


		_planes[SKYBOX_X_NEGATIVE].verts[i] = xneg[i];
		_planes[SKYBOX_Y_NEGATIVE].verts[i] = yneg[i];
		_planes[SKYBOX_Z_NEGATIVE].verts[i] = zneg[i];


		/* Copy Texture Coordinates */
		for (int j=0; j<SKYBOX_PLANES; j++)
			_planes[j].tex[i] = tex[i];
	}


	/* Load Textures */
	for (int i=0; i<SKYBOX_PLANES; i++)
	{
		/* Copy Filenames */
		sprintf(_planes[i].texFn, "textures/%s", fn[i]);

		/* Load Texture */
		_planes[i].texture = loadTexture(_planes[i].texFn);	
	}
}




void SKYBOX::render(CAMERA * c)
{
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(c->position().x, c->position().y, c->position().z);


	glDisable(GL_LIGHTING);

	/* Enable Texturing */
	glEnable(GL_TEXTURE_2D);


	/* Render the Planes */
	for (int i=0; i<SKYBOX_PLANES; i++)
	{
		glBindTexture(GL_TEXTURE_2D, _planes[i].texture);

		glBegin(GL_QUADS);
		for (int j=0; j<SKYBOX_VERTS; j++)
		{
			glTexCoord2fv((float*)&_planes[i].tex[j]);
			glVertex3fv((float*)&_planes[i].verts[j]);
		}
		glEnd();
	}
	
	glColor3f(1,1,1);

#if 0
	/* Enable Arrays */
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	/* Set Arrays */
	glVertexPointer(3, GL_FLOAT, 0, _vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, _textures);

	/* Draw Vertices */
	glDrawArrays(GL_QUADS, 0, _faces * 4);
	

	/* Disable Arrays */
	glDisableClientState( GL_VERTEX_ARRAY);
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
#endif

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);

	glPopMatrix();
}