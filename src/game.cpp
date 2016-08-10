/*
 * Name: Chris Snell      
 * Student ID: s3435406
 *
 */


#include "def.h"

#ifdef DEBUG
#include <stdio.h>
#endif


#include "game.h"


#include "terrain.h"
#include "camera.h"
#include "skier.h"
#include "skybox.h"
#include "trees.h"
#include "utility.h"
#include "particles.h"



player::~player()
{
	printf(": Player Destroyed\n");

	if (emitter)
		delete emitter;
	emitter = NULL;
	if (skier)
		delete skier;
	skier = NULL;
	if (camera)
		delete camera;
	camera = NULL;
}



/*
	GAME():
	Constructor
*/
GAME::GAME()
{
#ifdef DEBUG
	printf("Game Created\n");
#endif

	_ft = 0.0f;
	_ftTimer = 0.0f;
	_fps = 0;
	_fpsCounter = 0;

	_lighting = true;
	_displayAxis = true;
	_displayNormals = false;
	_displayBiNormals = false;
	_displayTangents = false;

	_displayPoints = false;
	_displayWireframe = false;

	_displaySlopes = false;


	_players = NULL;
	_terrain = NULL;

	

	/* Initialise Multiple Players */
	bool splitScreen = false;


	_playerCount = splitScreen ? PLAYERS_2 : PLAYERS_1;
	_players = new Player[_playerCount];
	for (int i=0; i<_playerCount; i++)
		_players[i] = Player();


	if (_playerCount == PLAYERS_1)
	{
		_players[0].skier = new SKIER();
		_players[0].camera = new CAMERA();
		_players[0].emitter = new EMITTER(PARTICLE_COUNT);
		_players[0].stream = 0;
	}
	else
	{		
		_players[0].skier = new SKIER(vec3(-10, 0, 0) );
		_players[0].camera = new CAMERA(region2(0, 0, 0.5f, 1) );
		_players[0].emitter = new EMITTER(PARTICLE_COUNT);
		_players[0].stream = 0;

		_players[1].skier = new SKIER(vec3(10, 0, 0) );
		_players[1].camera = new CAMERA(region2(0.5f, 0, 1, 1) );
		_players[1].emitter = new EMITTER(PARTICLE_COUNT);
		_players[1].stream = 1;
	}



	_terrain = new TERRAIN(_playerCount);
	_trees = new TREES();

	_skybox = new SKYBOX();
}

/*
	~GAME():
	Destructor
*/
GAME::~GAME()
{
#ifdef DEBUG
	printf("Game Destroyed\n");
#endif

	/* Delete Skybox */
	if (_skybox != NULL)
		delete _skybox;
	_skybox = NULL;

	/* Delete Trees */
	if (_trees != NULL)
		delete _trees;
	_trees = NULL;

	/* Delete Terrain */
	if (_terrain != NULL)
		delete _terrain;
	_terrain = NULL;

#if 0
	/* Delete Players */
	for (int i=0; i<_playerCount; i++)
	{
		if (_players[i].emitter)
			delete _players[i].emitter;
		if (_players[i].skier)
			delete _players[i].skier;
		if (_players[i].camera)
			delete _players[i].camera;

		_players[i].skier = NULL;
		_players[i].camera = NULL;
	}
#endif

	if (_players != NULL)
		delete[] _players;
	_players = NULL;
	_playerCount = 0;
}



/*
	resize():
	Resets the camera when game window changes
*/
void GAME::resize(int w, int h)
{
	/* Resize All Camera */
	for (int i=0; i<_playerCount; i++)
	{
		if (_players[i].camera)
			_players[i].camera->resize(w, h);
	}


	/* Enable Blending */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


/*
	getCamera():

	Gets the Camera that encapsulates the point p
*/
CAMERA * GAME::getCamera(point2 &p)
{
	for (int i=0; i<_playerCount; i++)
		if (_players[i].camera->inside(p))
			return _players[i].camera;

	return NULL;
}


/*
	getCamera():
*/
CAMERA * GAME::getCamera(int x, int y)
{
	point2 p = point2(x,y);
	return getCamera(p);	
}





/*
	keyboardPress():
	Keyboard Press Events
*/
void GAME::keyboardPress(unsigned char key, int x, int y)
{
	switch (key)
	{
	/* Toggle BiNormal Display */
	case KEY_B:
		_displayBiNormals = !_displayBiNormals;
		break;

	/* Toggle Points */
	case KEY_I:
		_displayPoints = !_displayPoints;
		break;

	/* Toggle Lighting */
	case KEY_L:
		_lighting = !_lighting;
		break;

	/* Toggle Normals */
	case KEY_N:
		_displayNormals = !_displayNormals;
		break;

	/* Toggle Axis */
	case KEY_O:
		_displayAxis = !_displayAxis;
		break;

	/* Toggle Wireframe */
	case KEY_P:
		_displayWireframe = !_displayWireframe;
		break;
	
	/* Display Tangents */
	case KEY_T:
		_displayTangents = !_displayTangents;
		break;

	/* Display Slopes */
	case KEY_U:
		_displaySlopes = !_displaySlopes;
		break;


	/*
		These need to be implemented in a better way
	*/
	case KEY_W:
		_players[0].skier->forward(true);
		break;
	case KEY_S:
		_players[0].skier->backward(true);
		break;
	case KEY_A:
		_players[0].skier->left(true);
		break;
	case KEY_D:
		_players[0].skier->right(true);
		break;
	}
}


/*
	keyboardRelease():
	Keyboard Release Events
*/
void GAME::keyboardRelease(unsigned char key, int x, int y)
{
	switch (key)
	{
	case KEY_W:
		_players[0].skier->forward(false);
		break;
	case KEY_S:
		_players[0].skier->backward(false);
		break;
	case KEY_A:
		_players[0].skier->left(false);
		break;
	case KEY_D:
		_players[0].skier->right(false);
		break;
	}
}


/*
	specialPress():
*/
void GAME::specialPress(int key, int x, int y)
{
	if (_playerCount < PLAYERS_2)
		return;

	switch (key)
	{
	case GLUT_KEY_UP:
		_players[1].skier->forward(true);
		break;
	case GLUT_KEY_DOWN:
		_players[1].skier->backward(true);
		break;
	case GLUT_KEY_LEFT:
		_players[1].skier->left(true);
		break;
	case GLUT_KEY_RIGHT:
		_players[1].skier->right(true);
		break;
	}
}

/*
	specialRelease():
*/
void GAME::specialRelease(int key, int x, int y)
{
	if (_playerCount < PLAYERS_2)
		return;

	switch (key)
	{
	case GLUT_KEY_UP:
		_players[1].skier->forward(false);
		break;
	case GLUT_KEY_DOWN:
		_players[1].skier->backward(false);
		break;
	case GLUT_KEY_LEFT:
		_players[1].skier->left(false);
		break;
	case GLUT_KEY_RIGHT:
		_players[1].skier->right(false);
		break;
	}
}





/*
	mouse():
	Mouse Clicky Events
*/
void GAME::mouse(int button, int state, int x, int y)
{
	_mouseLast.x = x;
	_mouseLast.y = y;

	_mouseDown[button] = ! state;
}


/*
	motion():
	Mouse Movement Events
*/
void GAME::motion(int x, int y)
{
	/* Do Region Checks for the Right Viewport */

	/* Rotate */
	if (_mouseDown[MOUSE_LEFT] == true)
	{
		int dx, dy;
		float mul = 0.5f;

		dx = x - _mouseLast.x;
		dy = y - _mouseLast.y;
	

		CAMERA *c = getCamera(x, y);

		if (c)
		{
			c->rotateTheta( c->rotateTheta() + dx * mul );
			c->rotatePhi( c->rotatePhi() + dy * mul );
		}
	}


	/* Zoom */
	if (_mouseDown[MOUSE_RIGHT] == true)
	{
		int dx;
		float d;
		float mul = 0.1f;
		dx = x - _mouseLast.x;

		d = dx * mul;

		
		CAMERA *c = getCamera(x, y);

		/* Zoom */
		if (c)
			c->zoom( c->zoom() + d );
	}

	/* Set Last Position */
	_mouseLast.x = x;
	_mouseLast.y = y;
}




/*
	update():
	Updates the Game
*/
void GAME::update(double ft)
{

	for (int i=0; i<_playerCount; i++)
	{
		/* Update the Player */
		_players[i].skier->update(ft, _terrain);

		/* Update Terrain */
		if (_terrain != NULL)
			_terrain->update(_players[i].camera, _players[i].stream);

		/* Update Particles */
		_players[i].emitter->update(_players[i].skier, (float)ft);
	}


	
	/* Collision Detection */
	for (int i=0; i<_playerCount; i++)
	{
		/* Tree Collisions */
		if (_trees)
		{
			for (int j=0; j<_trees->getTreeCount(); j++)
			{
				/* Get the Tree... adjusted to the player */
				tree tree = _trees->getTree(_terrain, _players[i].skier->position(), j);

				_players[i].skier->testCollision(tree.position, tree.size);
			}
		}

		/* 
			Player <-> Player Collisions 
			Seriously Bad.
		*/
		for (int j=i+1; j<_playerCount; j++)
		{
			_players[i].skier->testCollision(_players[j].skier->position(), _players[j].skier->radius());
			_players[j].skier->testCollision(_players[i].skier->position(), _players[i].skier->radius());
		}
	}




	/* _Update Timer */
	_ft = ft;

	_ftTimer += _ft;
	_fpsCounter++;

	if (_ftTimer > 1.0f)
	{
		_fps = _fpsCounter;
		_ftTimer -= 1.0f;
		_fpsCounter = 0;
	}
}


/*
	render():
	Renders the Game
*/
void GAME::render()
{
	/* Render each viewport */
	for (int i=0; i<_playerCount; i++)
		render(_players[i]);

	/* Disable Lighting */
	glDisable(GL_LIGHTING);


	/* Render Game World Axis */
	glDisable(GL_DEPTH_TEST);
	if (_displayAxis)
		renderAxis(50, 10, 50);
	glEnable(GL_DEPTH_TEST);





	/* Font Rendering */
	int x = 0;
	int y = 18;
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);

	/* Set Viewport */
	glViewport(0, 0, w, h);

	/* Push Matrices */
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, w, h, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	/* Disable Depth Tests */
	glDisable(GL_DEPTH_TEST);
	


	glTranslatef(0.0f, 0.0f, -1.0f );

	glColor3f(1.0f, 1.0f, 1.0f); 
	

	char s[80];

	sprintf(s, "FPS: %d", _fps);
	renderText(s, x, y);

	/* Re-enable Depth Tests */
	glEnable(GL_DEPTH_TEST);

	/* Pop Matrices */
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}


/*
	render():

	Render Each View Frame
*/
void GAME::render(player &player)
{
	/* Translate the Camera */
	player.camera->translate( player.skier->position() );
	player.camera->loadCamera();


	/* Render The Skybox - Furthest Away */
	glDisable(GL_LIGHT0);
	if (_skybox)
		_skybox->render(player.camera);


	/* Lighting */
	if (_lighting)
	{
		GLfloat white[] = {1,1,1,1};
		GLfloat pos[] = {2,1,0.5f,0};

		glEnable(GL_LIGHT0);
		glEnable(GL_NORMALIZE);	

		glLightfv(GL_LIGHT0, GL_AMBIENT, white);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
		glLightfv(GL_LIGHT0, GL_SPECULAR, white);
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
	}

	/* Wireframe Mode? */
	if (_displayWireframe)
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	else
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	/* Render Terrain - use stream 0 only for now */
	if (_terrain != NULL)
		_terrain->render(_lighting, player.stream);

	/* Render BOTH Player(s) */
	for (int i=0; i<_playerCount; i++)
		_players[i].skier->render(_lighting, _displayAxis);

	/* Render Trees */
	if (_trees)
		_trees->render(player.camera, _terrain);

	/* Render Particless (Camera is for the Viewport) */
	for (int i=0; i<_playerCount; i++)
		_players[i].emitter->render(player.camera);

	/* Turn off Lighting */
	if (_lighting && ! _displayWireframe)
		glDisable(GL_LIGHT0);

	/* Debug Rendering */
	if (_terrain != NULL)
	{
		/* Display Normals */
		if (_displayNormals)
			_terrain->renderNormals( player.stream );

		/* Display Tangents */
		if (_displayTangents)
			_terrain->renderTangents( player.stream);

		/* Display BiNormals */
		if (_displayBiNormals)
			_terrain->renderBiNormals( player.stream);

		/* Display Slopes */
		if (_displaySlopes)
			_terrain->renderSlopes( player.stream);
	}

	/* Push Matrices */
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, player.camera->viewWidth(), player.camera->viewHeight(), 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	/* Disable Depth Tests */
	glDisable(GL_DEPTH_TEST);
	
	int fontWidth = 9;
	int offset = 2;
	int textLen = 12;
	int numLen = 8;
	int len = textLen + numLen;

	int x = 0;
	int y = 18;

	if (player.stream == 0)
		x = player.camera->viewWidth()-( (len+offset) * fontWidth);
	else
		x = (offset) * fontWidth;

	glTranslatef(0.0f, 0.0f, -1.0f );
	glColor3f(1.0f, 1.0f, 1.0f); 
	
	char s[80];

	if (player.stream == 0)
	{
		sprintf(s, "%*s%*d", textLen, "Collisions:", numLen, player.skier->collisions());
		renderText(s, x, y);
		y+=18;

		sprintf(s, "%*s%*.1f", textLen, "Airtime:", numLen, player.skier->airTime());
		renderText(s, x, y);
		y+=18;
	}
	else
	{
		sprintf(s, "%*d%*s", -numLen, player.skier->collisions(), -textLen, ":Collisions" );
		renderText(s, x, y);
		y+=18;

		sprintf(s, "%*.1f%*s", -numLen, player.skier->airTime(), -textLen, ":Airtime" );
		renderText(s, x, y);
		y+=18;
	}


	/* Re-enable Depth Tests */
	glEnable(GL_DEPTH_TEST);

	/* Pop Matrices */
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();




	/* Re-enable Depth Tests */
	glEnable(GL_DEPTH_TEST);
}
