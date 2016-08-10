/*
 * Name: Chris Snell      
 * Student ID: s3435406
 *
 */



#include "def.h"

#ifdef DEBUG
#include <stdio.h>
#endif


#include "input.h"
#include "main.h"

#include "game.h"




GAME* game = NULL;


/*
	main():
*/
int main(int argc, char** argv)
{
	/* Set Cleanup Handler */
	atexit( cleanup );

	/* Initialise */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutInitWindowSize(1200, 600);
	glutCreateWindow("i3D: Assignment 2");

	
	/* Init Game Code */
	init();


	glEnable(GL_DEPTH_TEST);

	/* Setup Handlers */
	glutKeyboardFunc( keyboard );
	glutSpecialFunc( special );
	glutMouseFunc( mouse );
	glutMotionFunc( motion );

	/* freeglut */
	glutKeyboardUpFunc(keyboardRelease);
	glutSpecialUpFunc( specialRelease );

	glutReshapeFunc( resize );
	glutIdleFunc( update );
	glutDisplayFunc( render );
	
	

	glutMainLoop();


	return EXIT_SUCCESS;
}



/*
	init();
*/
void init()
{
#ifdef DEBUG
	printf(": init()\n");
#endif

	/* Create the Game */
	game = new GAME();
}



/*
	cleanup():
*/
void cleanup()
{
	if (game != NULL)
		delete game;
	game = NULL;

#ifdef DEBUG
	printf(": Cleanup()\n");
#endif



#ifdef DEBUG_TERMINATE
	getc(stdin);
#endif
}



/*
	resize():
*/
void resize(int w, int h)
{
#ifdef DEBUG
	printf(": Resize (%dx%d)\n", w, h);
#endif

	if (game != NULL)
		game->resize(w, h);
}



/*
	keyboardPress():
*/
void keyboard(unsigned char key, int x, int y)
{
#ifdef DEBUG_INPUT
	printf(": Keyboard Press: %d (%d, %d)\n", key, x, y);
#endif

	if (key == KEY_ESCAPE || key == KEY_Q)
		exit(0);
	else if (game != NULL)
		game->keyboardPress(key, x, y);
}


/*
	keyboardRelease():
*/
void keyboardRelease(unsigned char key, int x, int y)
{
#ifdef DEBUG_INPUT
	printf(": Keyboard Release: %d (%d, %d)\n", key, x, y);
#endif

	if (game != NULL)
		game->keyboardRelease(key, x, y);
}



/*
	specialPress():
*/
void special(int key, int x, int y)
{
#ifdef DEBUG_INPUT
	printf(": Special Press: %d (%d, %d)\n", key, x, y);
#endif

	if (game != NULL)
		game->specialPress(key, x, y);
}


/*
	specialRelease():
*/
void specialRelease(int key, int x, int y)
{
#ifdef DEBUG_INPUT
	printf(": Special Release: %d (%d, %d)\n", key, x, y);
#endif

	if (game != NULL)
		game->specialRelease(key, x, y);
}




/*
	mouse():
*/
void mouse(int button, int state, int x, int y)
{
#ifdef DEBUG_INPUT
	printf(": Mouse %d : %d (%dx%d)\n", button, state, x, y);
#endif 

	if (game != NULL)
		game->mouse(button, state, x, y);
}


/*
	motion():
*/
void motion(int x, int y)
{
#ifdef DEBUG_INPUT
	printf(": Motion: %dx%d\n", x, y);
#endif 

	if (game != NULL)
		game->motion(x, y);
}



/*
	update():
*/
void update()
{
	double t, dt;
	static double tlast = 0;

	/* Timing Code */
	t = glutGet(GLUT_ELAPSED_TIME);
	t /= 1000.0;

	dt = t - tlast;
	tlast = t;

	/* Update Game */
	if (game != NULL)
		game->update(dt);

	/* Redraw */
	glutPostRedisplay();
}

/*
	render():
*/
void render()
{
	int err;
	err = 0;

	/* Clear Backbuffer */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	/* Render Game */
	if (game != NULL)
		game->render();

	/* Swap Backbuffers for Display */
	glutSwapBuffers();


	/* Error Checks */
	while ( (err = glGetError()) != GL_NO_ERROR)
	{
		printf("error: %s\n", gluErrorString(err));
	}

}



