#ifndef _TREE_H_
#define _TREE_H_

#include "vectors.h"

class CAMERA;
class TERRAIN;


/*
	Tree

	Describes a single tree
*/
typedef struct tree
{
	vec3 position;
	float size;		/* Size of the Tree */
} Tree;


typedef struct tree_draw
{
	vec3 position;
	int index;

	float distance;
} TreeDraw;




/*
	TREES

	Manages trees
*/
class TREES
{
public:
	TREES();
	virtual ~TREES();

	void render(CAMERA *camera, TERRAIN *terrain);


	int getTreeCount() {return _count;}

	
	tree getTree(TERRAIN * terrain, const vec3 &v, const int index);

private:
	
	vec3 treePosition(TERRAIN *terrain, const region2 &r, const vec3 &cameraPos, const vec3 &treePos);
	float treeDistance(vec3 &cameraPos, vec3 &cameraDir, vec3 &treePos);

	unsigned int _texture;

	int _count;
	tree *_trees;
};

int treeComparison(const void *treeA, const void *treeB);


#endif