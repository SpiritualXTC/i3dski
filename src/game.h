/*
 * Name: Chris Snell      
 * Student ID: s3435406
 *
 * Class for Controlling Game
 */

#ifndef _GAME_H_
#define _GAME_H_

#include "def.h"


#include "vectors.h"
#include "input.h"

#define PLAYERS_1 1
#define PLAYERS_2 2
#define PARTICLE_COUNT 100


class CAMERA;
class TERRAIN;
class SKIER;
class TREES;
class SKYBOX;
class EMITTER;



typedef struct player
{
	player() {skier = NULL; camera = NULL; emitter = NULL;}
	~player();


	SKIER* skier;
	CAMERA* camera;

	EMITTER *emitter;

	unsigned int stream;
} Player;



class GAME
{
public:
	GAME();
	virtual ~GAME();

	bool getDisplayPoints()		{return _displayPoints;}
	bool getDisplayNormals()		{return _displayNormals;}
	bool getDisplayBiNormals()	{return _displayBiNormals;}
	bool getDisplayTangents()	{return _displayTangents;}

	inline bool getDisplayWireframe()	{return _displayWireframe;}

	void resize(int w, int h);

	void keyboardPress(unsigned char key, int x, int y);
	void keyboardRelease(unsigned char key, int x, int y);

	void specialPress(int key, int x, int y);
	void specialRelease(int key, int x, int y);

	void mouse(int button, int state, int x, int y);
	void motion(int x, int y);

	void update(double ft);
	void render();



private:

	CAMERA * getCamera(point2 &p);
	CAMERA * getCamera(int x, int y);

	void render(player &player);


	TERRAIN*	_terrain;
	TREES*		_trees;
	SKYBOX*		_skybox;

	int _playerCount;
	Player * _players;


	bool _lighting;

	bool _displayAxis;
	bool _displayPoints;
	bool _displayNormals;
	bool _displayBiNormals;
	bool _displayTangents;
	bool _displaySlopes;

	bool _displayWireframe;


	bool _mouseDown[MOUSE_BUTTONS];
	point2 _mouseLast;


	double _ft;
	double _ftTimer;
	int _fps;
	int _fpsCounter;
};




#endif
