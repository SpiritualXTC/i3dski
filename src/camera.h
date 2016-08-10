/*
 * Name: Chris Snell      
 * Student ID: s3435406
 *
 * Class for Controlling the Camera
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vectors.h"

class CAMERA
{
public:
	static const float CAMERA_ZOOM_MIN;


	CAMERA(region2 region = region2(0, 0, 1, 1));
	virtual ~CAMERA();

	void loadCamera();

	void resize(int w, int h);

	void zoom(float f);
	float zoom();

	void rotateTheta(float f);
	float rotateTheta();

	
	void rotatePhi(float f);
	float rotatePhi();

	void translate(float x, float y, float z);
	void translate(const vec3 &v);

	inline vec3 position() {return _translation;}

	inline int width()	{return _dimensions.x;}
	inline int height()	{return _dimensions.y;}

	inline int viewOffsetX() {return (int)(width() * _region.minimum.x);}
	inline int viewOffsetY() {return (int)(height() * _region.minimum.y);}

	inline int viewWidth()	{return (int)(width() * _region.width());}
	inline int viewHeight()	{return (int)(height() * _region.height());}

	inline float aspectRatio()	{return (float)viewWidth() / (float)viewHeight();}

	bool inside(point2 &p);

private:
	

	float _zoom;

	float _theta;
	float _phi;

	vec3 _translation;

	point2 _dimensions;	/* Screen Dimensions */
	region2 _region;	/* Screen Region */
};



#endif
