/*
 * Name: Chris Snell      
 * Student ID: s3435406
 *
 * Commonly used for functions
 */

#include "def.h"
#include "utility.h"
#include <stdlib.h>

color::color()
{
	r = 1.0f;
	g = 1.0f;
	b = 1.0f;
	a = 1.0f;
}

color::color(float fr, float fg, float fb)
{
	r = fr;
	g = fg;
	b = fb;
	a = 1.0f;
}

color::color(float fr, float fg, float fb, float fa)
{
	r = fr;
	g = fg;
	b = fb;
	a = fa;
}

color color::operator*(const float f)
{
	return color(r * f, g * f, b * f, a * f);
}



/*
	random():
	Random Number. Range=[0, 1]
*/
float fRandom()
{
	return (float)rand() / RAND_MAX;
}

/*
	random():
	Random Number. Range=[0, n]
*/
float fRandom(float n)
{
	return fRandom() * n;
}

/*
	random():
	Random Number. Range=[min, max]
*/
float fRandom(float min, float max)
{
	return fRandom() * (max-min) + min;
}



/*
	random():
	Random Number. Range=[0, n]
*/
int iRandom(int n)
{
	return rand() % n;
}

/*
	random():
	Random Number. Range=[min, max]
*/
int iRandom(int min, int max)
{
	return iRandom(max-min) + min;
}





/*
	renderAxis():
	Renders an Axis
*/
void renderAxis(const float len)
{
	renderAxis(len, len, len);
}

/*
	renderAxis():
	Renders an Axis, with variable length axes
*/
void renderAxis(const float x, const float y, const float z)
{
	glBegin(GL_LINES);

	//Draw Axis
	glColor3f(  1.0f, 0.0f, 0.0f);
	glVertex3f( 0.0f, 0.0f, 0.0f);
	glVertex3f( x, 0.0f, 0.0f);


	glColor3f(  0.0f, 1.0f, 0.0f);
	glVertex3f( 0.0f, 0.0f, 0.0f);
	glVertex3f( 0.0f, y, 0.0f);

	glColor3f(  0.0f, 0.0f, 1.0f);
	glVertex3f( 0.0f, 0.0f, 0.0f);
	glVertex3f( 0.0f, 0.0f, z);

	glEnd();
}

/*
	renderLine():
	Renders a line, from v0 to v1 in White
*/
void renderLine(const vec3 &v0, const vec3 &v1)
{
	renderLine(v0, v1, color(1,1,1));
}

/*
	renderLine():
	Renders a line, from v0 to v1 with color c
*/
void renderLine(const vec3 &v0, const vec3 &v1, const color &c)
{
	glBegin(GL_LINES);

	glColor3f(  c.r, c.g, c.b );
	glVertex3f( v0.x, v0.y, v0.z);
	glVertex3f( v1.x, v1.y, v1.z);

	glEnd();
}



/*
	renderBounds():
	Renders a Cylindrical set of lines around the position
*/
void renderBounds(const vec3 &position, const float radius, const float minY, const float maxY)
{
	int segments = 8;
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);

	glColor3f(1, 0, 0);
	glBegin(GL_LINE_LOOP);

	for (int i=0; i<segments; i++)
	{
		float a = (i/(float)segments) * (float)PI * 2.0f;
		glVertex3f(radius * cosf(a), minY, radius * sinf(a) );
	}

	for (int i=0; i<segments; i++)
	{
		float a = (i/(float)segments) * (float)PI * 2.0f;
		glVertex3f(radius * cosf(a), maxY, radius * sinf(a) );
	}

	glEnd();


	glPopMatrix();
}







/*
	renderText():
	Renders Text
*/
void renderText(const char *string, int x, int y)
{
	glRasterPos2f(x, y);
	
	for (int i=0; i<strlen(string); ++i)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);

	/*
	This function not supported by linux.... seriously???
	glutBitmapString(GLUT_BITMAP_9_BY_15, string);
	*/
}





/* Renders an Origin Plane */
void renderPlane(vec3 &n)
{
	vec3 v = vec3();
	renderPlane(n, v);
}

/* Renders a Plane */
void renderPlane(vec3 &n, vec3 &p)
{
	/* Calculate Tangent */

	vec3 c1 = n * vec3(0,0,1);
	vec3 c2 = n * vec3(0,1,0);
	vec3 c3 = n * vec3(1,0,0);

	vec3 tangent;// = c1.length() > c2.length() ? c1 : c2;

	if (c1.length() > c2.length())
	{
		if (c1.length() < c3.length())
			tangent = c3;
		else
			tangent = c1;//c2.length() > c3.length() ? c2 : c3;
	}
	else
	{
		if (c2.length() < c3.length())
			tangent = c2;
		else
			tangent = c3;
	}


	//vec3 tangent = n * vec3(0,0,1); Falls flat on it's face in some situations
	vec3 bitangent = n * tangent;

//	tangent.normalise();
//	bitangent.normalise();

	plane pl = plane(n);
	vec3 vertex, v1, v2;

	glBegin(GL_QUADS);

	v1 = n - tangent - bitangent;
	v2 = v1 + -n * 2;
	pl.intersect(vertex, v1, v2);
	vertex += p;
	glVertex3fv((float*)&vertex);


	v1 = n + tangent - bitangent;
	v2 = v1 -n * 2;
	pl.intersect(vertex, v1, v2);
	vertex += p;
	glVertex3fv((float*)&vertex);


	v1 = n + tangent + bitangent;
	v2 = v1 + -n * 2;
	pl.intersect(vertex, v1, v2);
	vertex += p;
	glVertex3fv((float*)&vertex);


	v1 = n - tangent + bitangent;
	v2 = v1 + -n * 2;
	pl.intersect(vertex, v1, v2);
	vertex += p;
	glVertex3fv((float*)&vertex);

	glEnd();




	/* Debug */
	renderLine(p, p + n);
	renderLine(p, p + tangent);
	renderLine(p, p + bitangent);
}


/* Renders an Origin Plane, Sets the Color */
void renderPlane(vec3 &n, const color &c)
{
	vec3 v = vec3();
	glColor3fv( (float*)&c );
	renderPlane(n, v);
}

/* Render a Plane, Sets the OpenGL Color */
void renderPlane(vec3 &n, vec3 &p, const color &c)
{
	glColor3fv( (float*)&c);
	renderPlane(n, p);
}
