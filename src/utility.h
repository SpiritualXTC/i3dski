/*
 * Name: Chris Snell      
 * Student ID: s3435406
 *
 * Collection of application wide common functions
 */

#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "vectors.h"

typedef struct color
{
	float r;
	float g;
	float b;
	float a;

	color();
	color(float r, float g, float b);
	color(float r, float g, float b, float a);

	color operator*(const float f);
} Color, COLOR;


float fRandom();
float fRandom(float n);
float fRandom(float min, float max);

int iRandom(int n);
int iRandom(int min, int max);


void renderAxis(const float len);
void renderAxis(const float x, const float y, const float z);

void renderLine(const vec3 &v0, const vec3 &v1);
void renderLine(const vec3 &v0, const vec3 &v1, const color &c);

void renderText(const char* string, int x, int y);


void renderPlane(vec3 &n);
void renderPlane(vec3 &n, const color &c);

void renderPlane(vec3 &n, vec3 &p);
void renderPlane(vec3 &n, vec3 &p, const color &c);



void renderBounds(const vec3 &position, const float radius, const float minY, const float maxY);


unsigned int buildSphere(unsigned int stacks, unsigned int slices, vec3 **v, vec3 **n);

#endif
