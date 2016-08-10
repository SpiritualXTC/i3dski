/*
 * Name: Chris Snell      
 * Student ID: s3435406
 * 
 * Main links to headers/libs
 */

#ifndef _DEF_H_
#define _DEF_H_

/* Core Library */
#include <stdlib.h>
#include <math.h>

/* Include Core Windows Functionality */
#if _WIN32
#include <Windows.h>
#endif

/* Include OpenGL Headers */
#include <GL/gl.h>
#include <GL/glu.h>


/* Glut Headers - Issue with 'Doze and standard glut */
#if _WIN32
#include <GL/freeglut_std.h>
#include <gl/freeglut_ext.h>
#else
#include <GL/glut.h>
#endif



/* Lazy Linker too GL Libraries. 'Doze only */
#ifdef _WIN32
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"freeglut.lib")
#endif


/* Global Debug Flag */
#ifndef DEBUG
#define DEBUG
#endif

/* Define Output */
#ifdef DEBUG
#define DEBUG_VERBOSE
#define DEBUG_CAMERA
#define DEBUG_TERRAIN
#define DEBUG_INPUT

#define DEBUG_PLAYER

#define DEBUG_PARTICLES
#define DEBUG_TREES

#define DEBUG_BOUNDS



	/* Flag to prevent application closing down completely when exiting (debugging) */
	/* Requires Input form the Terminal to Exit :'( */
	/* Not Needed for Linux, only for 'Doze-- and only then to look at terminal output */
#if _WIN32
#define DEBUG_TERMINATE
#endif




/* #undefine  */
#undef DEBUG_VERBOSE
#undef DEBUG_CAMERA
#undef DEBUG_INPUT
#undef DEBUG_TERRAIN

#undef DEBUG_PARTICLES
#undef DEBUG_TREES
#undef DEBUG_BOUNDS

#undef DEBUG_TERMINATE
#endif



/* Define Utility Constants & Macros */
#define PI 3.141593
#define DEGREES(r) ((r) * 180.0 / PI)
#define RADIANS(d) ((d) / 180.0 * PI)

#define MAX(m, n) ((m) > (n) ? (m) : (n))
#define MIN(m, n) ((m) < (n) ? (m) : (n))





#endif
