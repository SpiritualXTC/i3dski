#ifndef _MESH_H_
#define _MESH_H_


#include <vector>
#include "vectors.h"


typedef struct section
{
	section() {vertices = NULL;}
	~section()
	{
		if (vertices)
			free(vertices);

		vertices = NULL;
	}


	/* Material */
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;
	float shininess;

	/* Matrix */
	vec3 translation;
	vec3 rotation;

	/* Geometry */
	vec3 *vertices;

	int faces;

	std::vector<section> sections;
} Section;





class MESH
{
public:
	MESH();
	virtual ~MESH();

private:
	std::vector<section> sections;


};




#endif