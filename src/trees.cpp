#include "trees.h"

/*
	Tree.c

	Manages Trees.

	Currently Trees Are Alpha Blended.
	Trees Further Away are more Transparent.
	This is based on SORT ORDER. NOT DISTANCE!
*/
#include "def.h"
#include "camera.h"
#include "texture.h"
#include "terrain.h"
#include "utility.h"

#include <stdio.h>

TREES::TREES()
{
	_trees = NULL;

	_texture = loadTexture("textures/tree.png");

	float size = TERRAIN::GRID_SIZE;
	int cells = 8;

	float cellSize = size / cells;
	
	_count = cells * cells;
	_trees = (tree*)malloc(_count * sizeof(tree) );

	for (int i=0; i<_count; i++)
	{
		float x = (float)(i / cells);
		float z = (float)(i % cells);

		x = x * cellSize - size / 2;
		z = z * cellSize - size / 2;

		x += fRandom(cellSize);
		z += fRandom(cellSize);

		_trees[i].position = vec3(x, 0.0f, z);
		_trees[i].size = fRandom(2, 4);
	}
}


TREES::~TREES()
{
	if (_trees != NULL)
		free(_trees);
	_trees = NULL;

	deleteTexture(_texture);
	_texture = 0;
}




/*
	getTree():

	Gets the Tree @ index, relative to the position v
*/
tree TREES::getTree(TERRAIN * terrain, const vec3 &v, const int index)
{
	region2 r = terrain->calcRegion(v);

	tree t;

	vec3 p;
	p.y = 0.0f;
	p.x = ((int)v.x / (int)TERRAIN::GRID_SIZE) * TERRAIN::GRID_SIZE;
	p.z = ((int)v.z / (int)TERRAIN::GRID_SIZE) * TERRAIN::GRID_SIZE;

	t.position = treePosition(terrain, r, p, _trees[index].position);
	t.size = _trees[index].size;

	return t;
}





/*
	render():

	Back to Front Rendering using QSort.
	Render Trees.
*/
void TREES::render(CAMERA *camera, TERRAIN *terrain)
{
	/* Implement QSort */
	TreeDraw *trees = (TreeDraw*)malloc(_count * sizeof(TreeDraw) );
	if (! trees)
		return;

#if 0
	/* Estimates Render Region */
	vec3 center = terrain->calcCenter(camera);

	/* Way to calculate from terrain */
	float gs = TERRAIN::GRID_SIZE * 0.5f;
	region2 r = region2(center.x - gs, center.z - gs, center.x + gs, center.z + gs);	
#endif

	region2 r = terrain->calcRegion(camera);


	/* Camera Translation, Rounded to the TERRAIN Grids Center */
	vec3 cP = camera->position();
	vec3 cPos = camera->position();
	cPos.y = 0.0f;
	cPos.x = ((int)cPos.x / (int)TERRAIN::GRID_SIZE) * TERRAIN::GRID_SIZE;
	cPos.z = ((int)cPos.z / (int)TERRAIN::GRID_SIZE) * TERRAIN::GRID_SIZE;

	/* Camera Direction */
	vec3 cTan = vec3(cosf((float)RADIANS(camera->rotateTheta())), 0.0f, sinf((float)RADIANS(camera->rotateTheta())) );
	vec3 cDir = cTan * vec3(0,1,0); 

	for (int i=0; i<_count; i++)
	{
		trees[i].index = i;

		/* Tree Position */
		trees[i].position = treePosition(terrain, r, cPos, _trees[i].position);	
		
		/* Calculate Awesome Value for QSort Comparison */
		trees[i].distance = treeDistance(cP, cDir, trees[i].position);
	}

	/* Quick Sort Awesomeness */
	qsort(trees, _count, sizeof(TreeDraw), treeComparison);



	glMatrixMode(GL_MODELVIEW);

	/* Enable Texturing */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glColor4f(1, 1, 1, 1);

	/* Draw Individual Trees as Quads */
	for (int i=0; i<_count; i++)
	{
		TreeDraw *draw = &trees[i];
		Tree * tree = &_trees[draw->index];
	
		glColor4f(1, 1, 1, 1);

		/* Push a matrix */
		glPushMatrix();
		glTranslatef(draw->position.x, draw->position.y, draw->position.z);
		glScalef(tree->size, tree->size * 2.0f, tree->size);

		/* Align with Camera */
		glRotatef(-camera->rotateTheta(), 0, 1, 0);

		/* Draw Quad */
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, 2.0f, 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f( 1.0f, 2.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f( 1.0f, 0.0f, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 0.0f, 0.0f);

		glEnd();

		/* Pop Matrix */
		glPopMatrix();
	}

	/* Disable Texturing */
	glDisable(GL_TEXTURE_2D);



#ifdef DEBUG_BOUNDS
	for (int i=0; i<_count; i++)
	{
		TreeDraw *draw = &trees[i];
		Tree * tree = &_trees[draw->index];

		renderBounds(draw->position, tree->size, 0.0f, tree->size);
	}
#endif





	
#ifdef DEBUG_TREES
	glDisable(GL_DEPTH_TEST);

	float d = camera->position().dot(cDir);	
	vec3 dis = (cDir * d);
	vec3 v1, v2, v3;
	vec3 offset = vec3(0, camera->position().y, 0);

	glBegin(GL_LINES);

	glColor4f(1, 0, 1, 1);

	v1 = offset;
	v2 = dis + offset;

	glVertex3fv((float*)&v1);
	glVertex3fv((float*)&v2);


	glColor4f(0, 1, 1, 1);

	v1 = dis - cTan * 50.0f + offset;
	v2 = dis + cTan * 50.0f + offset;
		
	glVertex3fv((float*)&v1);
	glVertex3fv((float*)&v2);

	glEnd();


	glColor4f(1, 0, 1, 1);
	for (int i=0; i<_count; i++)
	{
		TreeDraw *draw = &trees[i];
		Tree * tree = &_trees[draw->index];
		glDisable(GL_DEPTH_TEST);

		glColor4f(1, 1, 0, 1);
		glBegin(GL_LINE_STRIP);

		v1 = trees[i].position;
		v2 = vec3(v1.x, camera->position().y, v1.z);
		v3 = v2 + cDir * -(trees[i].distance);

		glVertex3fv((float*)&v1);
		glVertex3fv((float*)&v2);
		glVertex3fv((float*)&v3);

		glEnd();
		glEnable(GL_DEPTH_TEST);
	}

	glEnable(GL_DEPTH_TEST);
#endif

	


}




/*
	treePosition

	Adjust the Trees Position, so it is within the region

	terrain: Terrain Handle
	r: Region around the Camera Position (Not-GRID Rounded)
	cameraPos: The GRID Rounded camera position
	treePos: Tree Position, in the non-translated grid

	returns:
		Region Aligned Tree Position
*/
vec3 TREES::treePosition( TERRAIN *terrain, const region2 &r, const vec3 &cameraPos, const vec3 &treePos)
{
	vec3 p = vec3( treePos.x, 0.0f, treePos.z);

	/* Translate Position to Camera */
	p += cameraPos;

	/* Tree Bounding */
	if (p.x < r.minimum.x)
		p.x += TERRAIN::GRID_SIZE;
	else if (p.x > r.maximum.x)
		p.x -= TERRAIN::GRID_SIZE;

	if (p.z < r.minimum.y)
		p.z += TERRAIN::GRID_SIZE;
	else if (p.z > r.maximum.y)
		p.z -= TERRAIN::GRID_SIZE;

	/* Retrieve Y-Coordinate for Tree */
	p.y = terrain->calcHeight(p.x, p.z);

	return p;
}


/*
	treeDistance():

	Calculates Distance to the Camera

	cameraPos: Camera Position (Not Grid-Rounded)
	cameraDir: Camera Direction (Ignores Y-Axis Rotations)
	treePos: Region Aligned Tree Position
*/
float TREES::treeDistance(vec3 &cameraPos, vec3 &cameraDir, vec3 &treePos)
{
	/*
		Theory:
		The Intersection of a Point (Tree Position) in the direction
		of the Camera's Tangent, will intersect the Camera's Direction Vector.

		Therefore:
			The Intersection Point will be ON the line, in the Camera's Direction.

		The Distance between the Intersection Point, and the Camera Position will
		be how far the tree is from the camera AND consequently, the View Plane

		A Dot Product of the Distance Vector * Camera Direction, will determine
		what side of the camera the tree is on.

		Doesn't take into account a flipped camera <,<
	*/

	vec3 c = vec3(cameraPos.x, 0.0f, cameraPos.z);
	vec3 t = vec3(treePos.x, 0.0f, treePos.z);

	/* As Ignoring any Y-Axis rotations for now, calculate a tangent via X-Prod */
	vec3 tangent = cameraDir * vec3(0,1,0);

	/* Plane Distance to Origin */
	float d = -c.dot(cameraDir);	

	float dot1 = tangent.dot(t);
	float dot2 = tangent.dot(tangent);

	/* Intersection Point */
	vec3 intersection = t + (tangent * ((d - dot1)/dot2));

	/* */
	float dir = cameraDir.dot(intersection - cameraPos) < 0.0f ? -1.0f : 1.0f;

	/* Distance from Intersection Point to Camera */
	return (intersection - cameraPos).length() * dir;
}














/*
	treeComparison():

	QSort Comparison Function.

*/
int treeComparison(const void *treeA, const void *treeB)
{
	TreeDraw *tA = (TreeDraw*)treeA;
	TreeDraw *tB = (TreeDraw*)treeB;

	return (int)( (tA->distance - tB->distance) * 10000);
}
