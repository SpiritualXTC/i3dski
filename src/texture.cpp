#include "texture.h"




#include "def.h"



#include <soil/SOIL.h>
#ifdef _WIN32
#pragma comment(lib, "soil.lib")
#endif

#include <stdio.h>




/*
	loadTexture():

	Loads a texture and return the OpenGL handle
*/
unsigned int loadTexture(char* pFilename)
{
	printf("Loading Texture: %s\n", pFilename);

	int width = 0;
	int height = 0;
	int channels = 0;

	unsigned int texture = 0;

	glGenTextures(1, (GLuint*)&texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	/* Load the Image */
	unsigned char *imgData = SOIL_load_image(pFilename, &width, &height, &channels, SOIL_LOAD_RGBA);

#ifdef DEBUG
	printf(": Dimension: %dx%d\n", width, height);
	printf(": Channels: %d\n", channels);
	printf(": Handle: %d\n", texture);
#endif

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/* Put to Vid Mem */
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData );

	/* Free Image */
	SOIL_free_image_data( imgData );

	return texture;
}


/*
	deleteTexture

	Deletes an OpenGL Texture
*/
bool deleteTexture(const unsigned int tex)
{
	if (tex == 0)
		return false;

#ifdef DEBUG
	printf(": Unloading Texture: Handle = %d\n", tex);
#endif

	glDeleteTextures(1, &tex);

	return true;
}