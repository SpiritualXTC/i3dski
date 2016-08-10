/*
 * Name: Chris Snell      
 * Student ID: s3435406
 *
 * Class for Terrain Rendering/Generation
 */

#ifndef _TERRAIN_H_
#define _TERRAIN_H_



#include "vectors.h"
#include "utility.h"

class CAMERA;

typedef struct stream
{
	stream();

	/* Should be able to use destructor but oh well */
	void freeStreams();	

	vec3* vertices;
	vec3* normals;
	vec2* texCoords;

	vec3* lineNormals;
	vec3* lineTangents;
	vec3* lineBiNormals;
	vec3* lineSlopes;

	vec3 last;
	vec3 textureTranslation;
} STREAM;


class TERRAIN
{
public:
	static const float GRID_SIZE;
	static const float GRID_STEP;


	static const float WAVEX_AMPLITUDE;
	static const float WAVEX_PHASE;
	static const float RIPPLE_AMPLITUDE;
	static const float RIPPLE_PHASE;
	static const float WAVEZ_AMPLITUDE;
	static const float WAVEZ_PHASE;
	static const float DESCENT;

	static const float NORMAL_LENGTH;

	static const float GENERATE_STEP;

	static const float TEXTURE_REPEAT;

	static const float GRAVITY;

	TERRAIN(unsigned int bufferStreams=1);
	virtual ~TERRAIN();






	void update(CAMERA * const camera, unsigned int stream);
	void render(bool lighting, unsigned int stream);

	void renderNormals(unsigned int stream);
	void renderTangents(unsigned int stream);
	void renderBiNormals(unsigned int stream);
	void renderSlopes(unsigned int stream);

	



	float calcHeight(const float x, const float z);
	vec3 calcNormal(const float x, const float z);
	vec3 calcSlope(const float x, const float z);

	float calculate(const float x, const float z, vec3 * const v, vec3 * const n);

	vec3 calcCenter(CAMERA * const camera); 

	region2 calcRegion(CAMERA * const camera);
	region2 calcRegion(const vec3 &center);


private:



	/* This Data should be the same across Terrain Streams */
	unsigned int*	_indices;
	unsigned int	_indiceCount;
	unsigned int	_vertexCount;

	vec2* _texCoords;	/* This is achieved with texture translation so remain the same */


	
	void generate();
	void generate(STREAM &stream);
	void regenerate(stream &stream, const vec3 &v, bool always=false);



	void render(stream &stream);
	void renderLines(color &color, vec3 *lines, unsigned int count);


	int _texture;

	STREAM *_streams;
	unsigned int _streamCount;
};




#endif
