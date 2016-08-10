
#include "camera.h"

#include "def.h"


const float CAMERA::CAMERA_ZOOM_MIN = 1.0f;

/* Constructor */
CAMERA::CAMERA(region2 region)
{
	_zoom = 16.0f;

	_theta = 0.0f;
	_phi = 45;

	_translation = vec3();
	_dimensions = point2();

	_region = region;
}


/* Destructor */
CAMERA::~CAMERA()
{



}

/*
	inside():

	Checks for a point inside the camera viewport region
*/
bool CAMERA::inside(point2 &p)
{
	if (p.x < viewOffsetX() || p.x > viewOffsetX() + viewWidth())
		return false;

	if (p.y < viewOffsetY() || p.y > viewOffsetY() + viewHeight())
		return false;

	return true;
}


/*
	resize():

	Resizes the Viewport
*/
void CAMERA::resize(int w, int h)
{
	_dimensions.x = w;
	_dimensions.y = h;

	glViewport(0, 0, viewWidth(), viewHeight());

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(75, aspectRatio(), 0.1f, 10000.0f);

	loadCamera();
}




/* Set Zoom Level */
void CAMERA::zoom(float f)
{
	_zoom = MAX(1.0f, f);
}

/* Get Zoom Level */
float CAMERA::zoom()
{
	return _zoom;
}


/* Translate the Camera */
void CAMERA::translate(float x, float y, float z)
{
	_translation.x = x;
	_translation.y = y;
	_translation.z = z;
}

/* Translate the Camera */
void CAMERA::translate(const vec3 &v)
{
	_translation = v;
}

/* Set Rotation around Y-Axis */
void CAMERA::rotateTheta(float f)
{
	_theta = f;
}

/* Get Rotation around Y-Axis */
float CAMERA::rotateTheta()
{
	return _theta;
}

/* Set Rotation Around X-Axis */
void CAMERA::rotatePhi(float f)
{
	_phi = f;
}

/* Get Rotation Around Y-Axis */
float CAMERA::rotatePhi()
{
	return _phi;
}

/* Reset the MODELVIEW matrix, and set the camera */
void CAMERA::loadCamera()
{
	glViewport(viewOffsetX(), viewOffsetY(), viewWidth(), viewHeight());

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(75, aspectRatio(), 0.1f, 10000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -zoom() );
	
	glRotatef(_phi, 1.0f, 0.0f, 0.0f);
	glRotatef(_theta, 0.0f, 1.0f, 0.0f);

	glTranslatef(-_translation.x, -_translation.y, -_translation.z);
}
