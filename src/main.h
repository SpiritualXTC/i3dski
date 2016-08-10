/*
 * Name: Chris Snell      
 * Student ID: s3435406
 *
 * Application Code
 */

#ifndef _MAIN_H_
#define _MAIN_H_




int main(int argc, char** argv);

void init();
void cleanup();
void resize(int w, int h);


void keyboard(unsigned char key, int x, int y);
void keyboardRelease(unsigned char key, int x, int y);
void special(int key, int x, int y);
void specialRelease(int key, int x, int y);


void mouse(int button, int state, int x, int y);
void motion(int x, int y);

void update();
void render();




#endif