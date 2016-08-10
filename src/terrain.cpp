/*
 * Name: Chris Snell      
 * Student ID: s3435406
 *
 */

#include "def.h"

/* Include IO for debugging only */
#ifdef DEBUG
#include <stdio.h>
#endif


#include "terrain.h"
#include "camera.h"

#include "texture.h"


stream::stream()
{
	vertices = NULL;
	normals = NULL;

	lineNormals = NULL;
	lineTangents = NULL;
	lineBiNormals = NULL;
	lineSlopes = NULL;

	last = vec3();
	textureTranslation = vec3();
}


void stream::freeStreams()
{
	if (lineSlopes != NULL)
		free( (void*)lineSlopes );
	lineSlopes = NULL;

	if (lineBiNormals != NULL)
		free( (void*)lineBiNormals );
	lineBiNormals = NULL;

	if (lineTangents != NULL)
		free( (void*)lineTangents );
	lineTangents = NULL;

	if (lineNormals != NULL)
		free( (void*)lineNormals );
	lineNormals = NULL;

	if (normals != NULL)
		free( (void*)normals );
	normals = NULL;

	if (vertices != NULL)
		free( (void*)vertices );
	vertices = NULL;
}








/* Setup Terrain Generation Constants */
const float TERRAIN::GRID_SIZE = 100.0f;
const float TERRAIN::GRID_STEP = 0.5f;

const float TERRAIN::WAVEX_AMPLITUDE = 4.0f;//2.0f;
const float TERRAIN::WAVEX_PHASE = 0.025f * 2.0f * (float)PI;
const float TERRAIN::WAVEZ_AMPLITUDE = 1.0f;	//2.0f
const float TERRAIN::WAVEZ_PHASE = 0.05f * 2.0f * (float)PI;

const float TERRAIN::RIPPLE_AMPLITUDE = 0.25f;
const float TERRAIN::RIPPLE_PHASE = 0.125f * 2.0f * (float)PI;

const float TERRAIN::DESCENT = 0.25f;//0.125f;//0.25f;

const float TERRAIN::NORMAL_LENGTH = 0.5f;

/* Alters how frequently the terrain is regenerated */
const float TERRAIN::GENERATE_STEP = 4.0;

const float TERRAIN::TEXTURE_REPEAT = 4.0f;
const float TERRAIN::GRAVITY = -9.8f;	



/* Flags to Enable/Disable Aspects of Terrain Generation */
bool TERRAIN_WAVEX = true;
bool TERRAIN_WAVEZ = true;
bool TERRAIN_RIPPLE = true;
bool TERRAIN_SLOPE = true;


/*
	Constructor
*/
TERRAIN::TERRAIN(unsigned int _bufferStreams)
{
#ifdef DEBUG
	printf("Terrain Created\n");
#endif


#if 0
	_vertices = NULL;
	_normals = NULL;
#endif

	_texCoords = NULL;
	


	_indices = NULL;

#if 0
	_lineNormals = NULL;
	_lineTangents = NULL;
	_lineBiNormals = NULL;
	_lineSlopes = NULL;

	_last = vec3(0,0,0);
	_textureTranslation = vec3(0,0,0);
#endif

	_indiceCount = 0;
	_vertexCount = 0;

	_texture = 0;




	/* Buffer Streams */
	_streamCount = _bufferStreams;
	_streams = new STREAM[_streamCount];

	/*
	_streams = (STREAM*)malloc(sizeof(STREAM) * _streamCount);
	for (unsigned int i=0; i<_streamCount; i++)
		_streams[i] = stream();
	*/


	/* Load the Texture */
	_texture = loadTexture("textures/terrain.png");

	/* Generate Terrain */
	generate();
}

/*
	Destructor
*/
TERRAIN::~TERRAIN()
{
#ifdef DEBUG
	printf("Terrain Destroyed\n");
#endif
	/* Free Memory */
	if (_indices != NULL)
		free( (void*)_indices );
	_indices = NULL;


	if (_texCoords != NULL)
		free( (void*)_texCoords );
	_texCoords = NULL;

	/* Free Streams */
	if (_streams)
	{
		for (unsigned int i=0; i<_streamCount; i++)
			_streams[i].freeStreams();

		delete[] _streams;
	}
	_streams = NULL;

	/* Unload Texture */
	deleteTexture(_texture);
	_texture = 0;

}


/*
	calculate():

	in:
		float x: X Position
		float y: Y Position
		
	out:
		vec3* v: Position on the Terrain (x,height,z) (Optional)
		vec3* n: Normal off the Terrain @ Position (Optional)

	return:
		float: Height @ x,z
*/
float TERRAIN::calculate(const float x, const float z, vec3 * const v, vec3 * const n)
{
	float h = calcHeight(x, z);

	if (n) *n = calcNormal(x, z);
	if (v) *v = vec3(x,h,z);

	return h;
}



/*
	calcHeight():
	Calculates height of the terrain @ position (x, z)
*/
float TERRAIN::calcHeight(const float x, const float z)
{
	float hwx = 0.0f, hwz = 0.0f, hr = 0.0f, hs = 0.0f;

	if (TERRAIN_WAVEX)
		hwx = TERRAIN::WAVEX_AMPLITUDE * sinf(x * TERRAIN::WAVEX_PHASE);
	if (TERRAIN_WAVEZ)
		hwz = TERRAIN::WAVEZ_AMPLITUDE * sinf(z * TERRAIN::WAVEZ_PHASE);

	if (TERRAIN_RIPPLE)
		hr = TERRAIN::RIPPLE_AMPLITUDE * sinf( (x + z) * TERRAIN::RIPPLE_PHASE);

	if (TERRAIN_SLOPE)
		hs = -TERRAIN::DESCENT * (x+z);

	return hwx + hwz + hr + hs;	
}


/*
	calcNormal():
	Calculates the Normal of the Terrain @ position (x,z)
*/
vec3 TERRAIN::calcNormal(const float x, const float z)
{
	float nwx = 0.0f, nwz = 0.0f, nr = 0.0f, ns = 0.0f;

	if (TERRAIN_WAVEX)
		nwx = -TERRAIN::WAVEX_PHASE * TERRAIN::WAVEX_AMPLITUDE * cosf(x * TERRAIN::WAVEX_PHASE);

	if (TERRAIN_WAVEZ)
		nwz = -TERRAIN::WAVEZ_PHASE * TERRAIN::WAVEZ_AMPLITUDE * cosf(z * TERRAIN::WAVEZ_PHASE);

	if (TERRAIN_RIPPLE)
		nr = -TERRAIN::RIPPLE_PHASE * TERRAIN::RIPPLE_AMPLITUDE * cosf( (x+z) * TERRAIN::RIPPLE_PHASE);

	if (TERRAIN_SLOPE)
		ns = TERRAIN::DESCENT;

	return vec3( nwx + nr + ns, 1.0f, nwz + nr + ns ).normalise();
}


/*
	calcSlope():

	Calculates a slope.
*/
vec3 TERRAIN::calcSlope(const float x, const float z)
{
	vec3 n = calcNormal(x, z);

	/*
		Probably a more effecient method for doing this!

		From a Position P, and a Normal N.
		Project a line from (P+N), in the direction of gravity, 
		until it intersects the Surface Denoted by N

		As all of the relevant vectors are positionless,
		P = (0,0,0) and P+N = N
	*/


	if (n.y == 1.0f)
		return vec3(0,0,0);

/* Calculate Tangent - Plane Equation */
/*
V = (0,0,0);
N = Normal @ Position

Nx(x - Vx) + Ny(y - Vy) + Nz(z - Vz) = 0
			
	Therefore:
Nx(x) + Ny(y) + Nz(z) = 0

	Along X-Axis
Vt = (x, y, z) = (1, ?, 0)

	Express Plane Eqn in terms of Ny(y)
Nx(x) + Nz(z) = -Ny(y)
(Nx(x) + Nz(z)) / -Ny = y;
*/

/*
	
*/


/*
Tangent from Plane Eqn
	float tx = 1.0f;
	float tz = 0.0f;
	t = vec3(tx, (n.x * tx + n.z * tz) / -n.y, tz).normalise();
*/
	return vec3(n.x, (n.x * n.x + n.z * n.z) / -n.y, n.z).normalise();
}


/*
	generate():
	Pregenerates an Indexable Vertex/Normal Array
*/
void TERRAIN::generate()
{
	unsigned int verts = (unsigned int)floorf(TERRAIN::GRID_SIZE / TERRAIN::GRID_STEP);
	unsigned int index=0;

	_vertexCount = (verts+1) * (verts+1);
	_indiceCount = verts * verts * 6;



	/* Create Vertex Streams */
	for (unsigned int i=0; i<_streamCount; i++)
		generate(_streams[i]);


#if 0
	/* Create Arrays */
	_vertices = (Vec3*)calloc( _vertexCount, sizeof(Vec3) ); 
	_normals = (Vec3*)calloc( _vertexCount, sizeof(Vec3) ); 
	//_texCoords = (Vec2*)calloc( _vertexCount, sizeof(Vec2));

	/* Create Line Arrays for Normals */
	_lineNormals = (Vec3*)calloc(_vertexCount * 2, sizeof(vec3) );
	_lineTangents = (Vec3*)calloc(_vertexCount * 2, sizeof(vec3) );
	_lineBiNormals = (Vec3*)calloc(_vertexCount * 2, sizeof(vec3) );
	_lineSlopes = (Vec3*)calloc(_vertexCount * 2, sizeof(vec3) );
#endif


	/* Create Texture Coordinates Buffer */
	_texCoords = (Vec2*)calloc( _vertexCount, sizeof(Vec2));

	/* Create Indices Buffer */
	_indices = (unsigned int*)calloc(_indiceCount, sizeof(unsigned int) );


	/* Generate Texture Coordinates */
	vec2* ptex = _texCoords;
	for (unsigned int j=0; j<=verts; j++)
	{
		for (unsigned int i=0; i<=verts; i++)
			*(ptex++) = vec2( ((float)i/verts) * TERRAIN::TEXTURE_REPEAT, ((float)j/verts) * TERRAIN::TEXTURE_REPEAT);
	}


	/* Generate Indices */
	for (unsigned int j=0; j<verts; j++)
	{
		for (unsigned int i=0; i<verts; i++)
		{
			_indices[index++] = i + 0 + (j + 0) * (verts + 1);				
			_indices[index++] = i + 1 + (j + 0) * (verts + 1);			
			_indices[index++] = i + 0 + (j + 1) * (verts + 1);				

			_indices[index++] = i + 0 + (j + 1) * (verts + 1);
			_indices[index++] = i + 1 + (j + 0) * (verts + 1);
			_indices[index++] = i + 1 + (j + 1) * (verts + 1);	


#ifdef DEBUG_VERBOSE
			printf("%d\t[%d\t%d\t%d]\n", index, 
				_indices[index - 6], _indices[index - 5], _indices[index - 4]);
			printf("%d\t[%d\t%d\t%d]\n", index, 
				_indices[index - 3], _indices[index - 2], _indices[index - 1]);
#endif
		}
	}

	/* Populate Vertex Buffers */
#if 0
	vec3 v = vec3(0,0,0);
	regenerate(v);
#endif



}


/*
	generate():
	Generate vertex information for the stream
*/
void TERRAIN::generate(STREAM &stream)
{
	/* Create Arrays */
	stream.vertices			= (Vec3*)calloc( _vertexCount, sizeof(Vec3) ); 
	stream.normals			= (Vec3*)calloc( _vertexCount, sizeof(Vec3) ); 

	/* Create Line Arrays for Normals */
	stream.lineNormals		= (Vec3*)calloc(_vertexCount * 2, sizeof(vec3) );
	stream.lineTangents		= (Vec3*)calloc(_vertexCount * 2, sizeof(vec3) );
	stream.lineBiNormals	= (Vec3*)calloc(_vertexCount * 2, sizeof(vec3) );
	stream.lineSlopes		= (Vec3*)calloc(_vertexCount * 2, sizeof(vec3) );

	regenerate(stream, vec3(), true);
}






/*
	regenerate(vec3 v):
	v:		Center point of the Terrain Grid to generate around

	Set Vertex/Normal Data
*/
void TERRAIN::regenerate(stream &stream, const vec3 &v, bool always)
{
	/* Calculate Start/Finish Values */
	float x, z;
	float xs, xf, zs, zf;



	/* Haven't Moved Enough since last update */
	if (always == false && (fabs(v.x - stream.last.x) <= TERRAIN::GENERATE_STEP && fabs(v.z - stream.last.z) <= TERRAIN::GENERATE_STEP))
		return;


	/* Calculate Texture Translations */
	float tStep = TERRAIN::GRID_STEP / TERRAIN::GRID_SIZE;
	float tStepRep = (TERRAIN::GRID_STEP / TERRAIN::GRID_SIZE) * TERRAIN::TEXTURE_REPEAT;
	float tu = (floor(v.x / tStepRep) * tStepRep) * tStepRep / TERRAIN::GRID_STEP;
	float tv = (floor(v.z / tStepRep) * tStepRep) * tStepRep / TERRAIN::GRID_STEP;

	stream.textureTranslation = vec3(tu, tv, 0.0f);


	/* Calculate Start/End Tiling Values */
	xs = floorf(v.x) - TERRAIN::GRID_SIZE / 2;
	zs = floorf(v.z) - TERRAIN::GRID_SIZE / 2;
	xf = xs + TERRAIN::GRID_SIZE;
	zf = zs + TERRAIN::GRID_SIZE;


	/* Obtain Pointers */
	vec3 * normals = stream.normals;
	vec3 * vertices = stream.vertices;

	vec3 * lNormals = stream.lineNormals;
	vec3 * lTangents = stream.lineTangents;
	vec3 * lBiNormals = stream.lineBiNormals;
	vec3 * lSlopes = stream.lineSlopes;


	for (z=zs; z<=zf; z+=TERRAIN::GRID_STEP)
	{
		for (x=xs; x<=xf; x+=TERRAIN::GRID_STEP)
		{
			vec3 p, n, t, b, s;

			/* Calculate Position & Normal */
			calculate(x, z, &p, &n);

			/* Calculate Tangent - Plane Equation */
			/*
			V = (0,0,0);
			N = Normal @ Position

			Nx(x - Vx) + Ny(y - Vy) + Nz(z - Vz) = 0
			
				Therefore:
			Nx(x) + Ny(y) + Nz(z) = 0

				Along X-Axis
			Vt = (x, y, z) = (1, ?, 0)

				Express Plane Eqn in terms of Ny(y)
			Nx(x) + Nz(z) = -Ny(y)
			(Nx(x) + Nz(z)) / -Ny = y;
			*/

		//	float tx = 1.0f;
		//	float tz = 0.0f;
		//	t = vec3(tx, (n.x * tx + n.z * tz) / -n.y, tz).normalise();

			/* Cross Product Tangent, now that the X-Prod code is working properly *RAGE* */
			t = n * vec3(0,0,1);

			/* BiNormal Vector - Cross Prod of Normal & Tangent */
			b = t * n;

			/* Set Vertex & Normal Data */
			*(vertices++) = p;
			*(normals++) = n;

			*(lNormals++) = p;
			*(lNormals++) = p + n * TERRAIN::NORMAL_LENGTH;

			*(lTangents++) = p;
			*(lTangents++) = p + t * TERRAIN::NORMAL_LENGTH;

			*(lBiNormals++) = p;
			*(lBiNormals++) = p + b * TERRAIN::NORMAL_LENGTH;

			/* Calculate Slope Vector */
			s = calcSlope(x, z);

			*(lSlopes++) = p;
			*(lSlopes++) = p + s * TERRAIN::NORMAL_LENGTH;
		}
	}

	/* Set Last Position */
	stream.last = v;

#ifdef DEBUG_TERRAIN
	printf(": Terrain Regenerate @ (%.2f, %.2f)\n", stream.last.x, +stream.last.z);
#endif
}



/*
	calcCenter():
	Calculates the Center Point for the Terrain Buffers.

	Projects a Point in the direction of the camera, from the Camera Position
	to calculate where the center point is for terrain generation
*/
vec3 TERRAIN::calcCenter(CAMERA * const camera)
{
#ifdef DEBUG_CAMERA
	return camera->position();
#endif

	const float CAMERA_DISTANCE = 30.0f;

	vec3 d = vec3(cosf((float)(RADIANS(camera->rotateTheta()) - PI/2.0f)), 0.0f, sinf((float)(RADIANS(camera->rotateTheta()) - PI/2.0f)));

	return d * CAMERA_DISTANCE + camera->position();
}


/*
	calcRegion():

	Calculates a Terrain Region from the camera
*/
region2 TERRAIN::calcRegion(CAMERA * const camera)
{
	vec3 center = calcCenter(camera);

	return calcRegion(center);
}


/*
	calcRegion():

	Calculates a Terrain Region using an 
	Arbiturary Coordinate
*/
region2 TERRAIN::calcRegion(const vec3 &center)
{
	float gs = TERRAIN::GRID_SIZE * 0.5f;
	return region2(center.x - gs, center.z - gs, center.x + gs, center.z + gs);	
}



/*
	update():
	Update Terrain
*/
void TERRAIN::update(CAMERA * const camera, unsigned int stream)
{
	vec3 p = calcCenter(camera);

	/* Stream Validation */
	if (stream < 0 || stream >= _streamCount)
		return;

	/* Regenerate Buffer */
	regenerate(_streams[stream], p);
}



/*
	render():
	Render Terrain
*/
void TERRAIN::render(bool lighting, unsigned int stream)
{
	/* Stream Validation */
	if (stream < 0 || stream >= _streamCount)
		return;

	GLfloat black[] = {0,0,0,0};
	GLfloat dif[] = {1.0f,1.0f,1.0f,1.0f};
	GLfloat shiny = 128.0f;

	/* Set Material */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

	/* Lighting */
	if (lighting)
		glEnable(GL_LIGHTING);
	else
		glColor3fv(dif);


	/* Render Buffers */

	/* GL_TEXTURE_MATRIX */

	render(_streams[stream]);
#if 0
	/* Enable Texturing */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _texture);

	


	/* Enable Arrays */
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_INDEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	/* Set Arrays */
	glNormalPointer(GL_FLOAT, 0, _normals);
	glVertexPointer(3, GL_FLOAT, 0, _vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, _texCoords);

	/* Texture Matrix Transformation */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glPushMatrix();

	glTranslatef(_textureTranslation.x, _textureTranslation.y, _textureTranslation.z);

	/* Draw Vertices */
	glDrawElements(GL_TRIANGLES, _indiceCount, GL_UNSIGNED_INT, _indices);


	glPopMatrix();





	#endif
	/* Lighting */
	if (lighting) glDisable(GL_LIGHTING);
}


/*
	render():

	Render a Terrain Stream
*/
void TERRAIN::render(stream &stream)
{
	/* Enable Texturing */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _texture);

	/* Enable Arrays */
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_INDEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	/* Set Arrays */
	glNormalPointer(GL_FLOAT, 0, stream.normals);
	glVertexPointer(3, GL_FLOAT, 0, stream.vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, _texCoords);

	/* Texture Matrix Transformation */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glPushMatrix();

	glTranslatef(stream.textureTranslation.x, stream.textureTranslation.y, stream.textureTranslation.z);

	/* Draw Vertices */
	glDrawElements(GL_TRIANGLES, _indiceCount, GL_UNSIGNED_INT, _indices);

	/* Disable Arrays */
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_INDEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY);
	glDisableClientState( GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}


/*
	renderLines():
	Renders the Line Buffers
*/
void TERRAIN::renderLines(color &color, vec3 *lines, unsigned int count)
{
	glDisable(GL_DEPTH_TEST);

	glColor4f(color.r, color.g, color.b, color.a);

	glEnableClientState( GL_VERTEX_ARRAY );

	glVertexPointer(3, GL_FLOAT, 0, lines);
	glDrawArrays(GL_LINES, 0, count * 2);

	glEnableClientState( GL_VERTEX_ARRAY );

	glEnable(GL_DEPTH_TEST);
	return;
}





/*
	renderNormals():
	Render Terrain Normals
*/
void TERRAIN::renderNormals(unsigned int stream)
{
	color c = color(1,1,0,1);	

	/* Stream */
	if (stream < 0 || stream >= _streamCount)
		return;

	renderLines(c, _streams[stream].lineNormals, _vertexCount);
}


/*
	renderTangents():
	Render Tangent Lines
*/
void TERRAIN::renderTangents(unsigned int stream)
{
	color c = color(0,1,1,1);	

	/* Stream */
	if (stream < 0 || stream >= _streamCount)
		return;

	renderLines(c, _streams[stream].lineTangents, _vertexCount);
}

/*
	renderBiNormals():
	Render BiNormal Lines
*/
void TERRAIN::renderBiNormals(unsigned int stream)
{
	color c = color(1,0,1,1);	

	/* Stream */
	if (stream < 0 || stream >= _streamCount)
		return;

	renderLines(c, _streams[stream].lineBiNormals, _vertexCount);
}


/*
	renderSlopes():
	Render Slopes
*/
void TERRAIN::renderSlopes(unsigned int stream)
{
	color c = color(1,0,0.5f,1);	

	/* Stream */
	if (stream < 0 || stream >= _streamCount)
		return;

	renderLines(c, _streams[stream].lineSlopes, _vertexCount);
}
