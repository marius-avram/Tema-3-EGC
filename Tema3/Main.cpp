//include librarii de opengl, glut si glu
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")

//includes
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>

#include "glut.h"
#include "Game.h"




int main(int argc, char *argv[]){

	//init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	//init window
	
	glutInitWindowSize(Game::width, Game::height);
	glutInitWindowPosition(Game::offsetx, Game::offsety);
	glutCreateWindow("Labyrinth");
	glutFullScreen();
	Game::init();

	//callbacks
	glutDisplayFunc(Game::display);
	glutReshapeFunc(Game::reshape);
	glutKeyboardFunc(Game::keyboard);
	glutMouseFunc(Game::mouseKey);
	glutPassiveMotionFunc(Game::mouseMovement);
	glutIdleFunc(Game::idle);
	

	glEnable(GL_DEPTH_TEST);

	//set background
	glClearColor(0.2,0.2,0.2,1.0);


	//loop
	glutMainLoop();

	return 0;
}