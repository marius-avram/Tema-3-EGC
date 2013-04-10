#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h> 
#include "Game.h"
#include "Character.h"
#include "Portal.h"
#include "Bonus.h"
#include "Labyrinth.h"

int Game::offsetx = 0;
int Game::offsety = 0;
int Game::width = 1366;
int Game::height = 768;
int Game::camera_mode = 0;
float Game::total_camera_angleY = 0.0f;
float Game::MOUSE_SPEED = 0.001f;
float Game::MOVING_SPEED = 0.3f;
float Game::TPS_CAMERA_RADIUS = 10.0f;
float Game::COLLISION_RADIUS = 1.2f;
float Game::transparency = 1.0f;
float Game::char_transparency = 1.0f;
DynamicCamera Game::dynamic_camera(TPS_CAMERA_RADIUS);
StaticCamera Game::static_camera;
Portal *Game::portal = NULL;
Bonus *Game::wall_bonus = NULL;
Bonus *Game::destroy_bonus = NULL;
Character *Game::character = NULL;
Labyrinth *Game::labyrinth = NULL;
char Game::hud_text[60];
bool Game::canRestart = false;
bool Game::canDestroy = false;
clock_t Game::wallb_start = 0.0f;
clock_t Game::destroyb_start = 0.0f;

void Game::init(){
	// Resetem timerele pentru a ne asigura sa nu avem text din 
	// rundele anterioare de joc. De asemnea restam si restul 
	// parametrilor
	wallb_start = 0.0f; 
	destroyb_start = 0.0f;
	canDestroy = false;
	canRestart = false;
	transparency = 1.0f;
	char_transparency = 1.0f;

	glutSetCursor(GLUT_CURSOR_NONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	// Initializarea celor doua tipuri de camere
	Game::dynamic_camera.init();
	Game::static_camera.init();

	// Initializare labirint
	labyrinth = new Labyrinth(dynamic_camera, &transparency);
	// Initializare caracter, portal, bonusuri
	character = new Character(dynamic_camera, &char_transparency);
	portal = new Portal(5.0f, labyrinth->getPortalRotation(), 
						labyrinth->getPortalPosition(), dynamic_camera);
	wall_bonus = new Bonus(0.5f, labyrinth->getWallBonusPosition(), dynamic_camera);
	destroy_bonus = new Bonus(0.5f, labyrinth->getDestroyBonusPosition(),
					dynamic_camera);
	destroy_bonus->setColor(0.0f, 0.0f, 1.0f);
	
	// Daca jocul a fost restartat si camera era setata 
	// in modul TPS atunci se seteaza camera in modul FPS 
	// si se reseteaza toate rotatiile din runda anterioara
	if (dynamic_camera.isTPSCamera()){
		camera_mode = 1;
		dynamic_camera.setFPSCamera();
		dynamic_camera.resetTPSCamera();
	}
	dynamic_camera.setPosition(labyrinth->getCameraPosition());
	dynamic_camera.rotateFPS_OY(labyrinth->getCameraRadRotation());
	total_camera_angleY = labyrinth->getCameraRadRotation();
	

	// Initializare lumina
	float position4f[] = {0.0, 0.0f, 1.0f, 0.0f};
	float color4f[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_DIFFUSE, color4f);
	glLightfv(GL_LIGHT0, GL_POSITION, position4f);
	glEnable(GL_LIGHT0);
	

	// Resetare text ce trebuie afisat pe ecran
	resetHudText();

}

// Elibereaza resursele alocate in init. Acestea trebuie 
// eliberate cand se restarteaza jocul pentru ca se apeleaza
// inca o data init si alte noi resurse sunt alocate
void Game::freeResources(){
	delete labyrinth;
	delete character;
	delete portal;
}


//functie afisare
void Game::display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	reshape(width, height);
	//setup view
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (camera_mode==3){
		static_camera.render();
	}
	else {
		dynamic_camera.render();
	}
	

	// Desenare portal iesire 
	portal->draw();

	// Desenare bonusuri 
	if (wall_bonus!=NULL){
		wall_bonus->draw();
	}
	if (destroy_bonus!=NULL){
		destroy_bonus->draw();
	}

	// Desenare labirint
	labyrinth->draw();
	labyrinth->isCollidingWall(1.2f);
	
	// Desenare personaj principal
	Vector3D camera_position = dynamic_camera.getPosition();

	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
		glTranslatef(camera_position.x, camera_position.y-1.0f, camera_position.z);
		glRotatef(180, 0.0f, 1.0f, 0.0f);
		glRotatef((float)(-total_camera_angleY * 180 / M_PI), 0.0f, 1.0f, 0.0f);
		if (camera_mode == 2){
			// Daca suntem in modul TPS plasam obiectul mai in fata
			// relativ la camera, deoarece prima tanslatie depinde de
			// pozitia camerei. Iar in modul TPS camera este translata
			// in spate cu 10.
			glTranslatef(0, 0, TPS_CAMERA_RADIUS);
		}
		character->draw();
	glPopMatrix();


	countdownTransparency();
	countdownBlockDestruction();
	displayHud();
	
	//swap buffers
	glutSwapBuffers();
}

void Game::reshape(int width, int height){
	//set viewport
	glViewport(0,0,width,height);
	
	//set proiectie
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(float)width/(float)height,0.2,150);
	
}

void Game::displayHud(){
	if (strcmp(hud_text, "")!=0){
		// Dezactivam testul de adancime
		glDisable(GL_DEPTH_TEST);
		// Setam alt viewport
		glViewport(10, height-30, 5, 5); 
		
		// Setam proiectie ortografica
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, width-1.0, 0.0, height-1.0, -1.0, 1.0);
		// Afisam textul
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		printText(0.0f, 0.0f, hud_text);
		glEnable(GL_DEPTH_TEST);
	}
}


void Game::keyboard(unsigned char ch, int x, int y){
	switch(ch){
		case 27:	//esc
			exit(0);
			break;

		case 'w':
			dynamic_camera.translate_Forward(MOVING_SPEED);
			character->updatePosition();
			if (labyrinth->isCollidingWall(COLLISION_RADIUS)){
				// Daca se detecteaza coliziune dupa efectuarea 
				// unei translatari se readuce la pozitia anterioara
				dynamic_camera.translate_Forward(-MOVING_SPEED);
			}
			checkBonusPick();
			isgameOver();
			break;
		case 'a':
			dynamic_camera.translate_Right(-MOVING_SPEED);
			character->updatePosition();
			if (labyrinth->isCollidingWall(COLLISION_RADIUS)){
				// Se verifica coliziunea
				dynamic_camera.translate_Right(MOVING_SPEED);
			}
			checkBonusPick();
			isgameOver();
			break;
		case 's':
			dynamic_camera.translate_Forward(-MOVING_SPEED);
			character->updatePosition();
			if (labyrinth->isCollidingWall(COLLISION_RADIUS)){
				// Se verifica coliziunea
				dynamic_camera.translate_Forward(MOVING_SPEED);
			}
			checkBonusPick();
			isgameOver();
			break;
		case 'd':
			dynamic_camera.translate_Right(MOVING_SPEED);
			character->updatePosition();
			if (labyrinth->isCollidingWall(COLLISION_RADIUS)){
				// Se verifica coliziunea
				dynamic_camera.translate_Right(-MOVING_SPEED);
			} 
			checkBonusPick();
			isgameOver();
			break;
		case 't': 
			dynamic_camera.setRotationTPS_OX(MOUSE_SPEED*30, 10.0f);
			break; 
		case 'g': 
			dynamic_camera.setRotationTPS_OX(-MOUSE_SPEED*30, 10.0f);
			break;
		case 'e':
			dynamic_camera.setRotationTPS_OZ(MOUSE_SPEED*30, 10.0f);
			break;
		case 'q': 
			dynamic_camera.setRotationTPS_OZ(-MOUSE_SPEED*30, 10.0f);
			break;
		case 'c':
			dynamic_camera.setRotationTPS_OY(MOUSE_SPEED*30, 10.0f);
			break;
		case 'z':
			dynamic_camera.setRotationTPS_OY(-MOUSE_SPEED*30, 10.0f);
			break;
		case 'r': 
			dynamic_camera.resetTPSCamera();
			break;
		case 'y':
			if (canRestart){
				freeResources();
				init();
			}
			break;
		case 'n':
			if (canRestart){
				exit(0);
			}
			break;
		case '1':
			camera_mode = 1;
			dynamic_camera.setFPSCamera();
			break;
		case '2':
			camera_mode = 2;
			dynamic_camera.setTPSCamera();
			break;
		case '3':
			if (camera_mode==2){
				dynamic_camera.setFPSCamera();
			}
			camera_mode = 3;
			break;
		default:
			break;
	}

}

// Cu ajutorul mouseul miscam camera pe axa oy
void Game::mouseMovement(int x, int y){
	int offsetx = x - width/2;
	float camera_angleY = offsetx*MOUSE_SPEED;
	if (camera_mode!=0){

		total_camera_angleY += camera_angleY;
		if (total_camera_angleY > 360.0f){
			total_camera_angleY -= 360.0f;
		}
		if (total_camera_angleY < -360.0f){
			total_camera_angleY += 360.0f;
		}


		if (camera_mode==2){
			dynamic_camera.rotateTPS_OY(camera_angleY, 10);
		}
		else { 
			// Camera dinamica se comporta la fel atat in camera 
			// mode 1 cat si in camera mode 3. In camera mode 3, 
			// camera dinamica nu este renderizanta. Insa ea trebuie
			// modificata pentru ca mutarile personajului sunt 
			// relative la camera dinamica.
			dynamic_camera.rotateFPS_OY(camera_angleY);
		}
	}
}

void Game::mouseKey(int button, int state, int x, int y){
	if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
		if (canDestroy){
			// Se distrug blocuri doar daca a fost luata 
			// puterea care da aceasta abilitate
			labyrinth->destoryLastCollided();
		}
	}
}


void Game::idle(){
	glutWarpPointer(width/2, height/2);
	glutPostRedisplay();
	
}

int Game::getCameraMode(){
	return camera_mode;
}

void Game::printText(float x, float y, char *text){
	if (!text || !strlen(text)) return;
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(x,y);
	while(*text){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *text);
		text++;
	}
}


void Game::setHudText(char *text){
	// Daca textul pe care dorim sa il afisam
	// este diferit de cel care este afisat in 
	// momentul de fata atunci il modificam
	if (strcmp(hud_text, text)!=0){
		sprintf(hud_text, text);
	}
}

void Game::resetHudText(){
	sprintf_s(hud_text, "");
}

// Verifica daca jocul s-a terminat (jucatorul a ajuns la portal).
void Game::isgameOver(){
	if (portal->insideObject(dynamic_camera.getPosition(), COLLISION_RADIUS)){
		char_transparency = 0.0f;
		setHudText("**Felicitari! Ai gasit Iesirea.** Restart?(Y/N)");
		canRestart = true;
	}
}

void Game::checkBonusPick(){
	if (wall_bonus!=NULL){
		if (wall_bonus->insideObject(dynamic_camera.getPosition(), COLLISION_RADIUS)){
			delete wall_bonus;
			wall_bonus = NULL;
			transparency = 0.25f;
			wallb_start = clock();
			if (canDestroy==true){
				canDestroy = false;
				destroyb_start = 0.0f;
			}
		}
	}
	if (destroy_bonus!=NULL){
		if (destroy_bonus->insideObject(dynamic_camera.getPosition(), COLLISION_RADIUS)){
			delete destroy_bonus;
			destroy_bonus = NULL;
			canDestroy = true;
			destroyb_start = clock();
			if (transparency==0.25f){
				transparency = 1.0f;
				wallb_start = 0.0f;
			}
		}
	}
}

void Game::countdownTransparency(){
	clock_t timer = 8000.0f;
	clock_t now;
	float timer_disp;
	if (wallb_start!=0 && transparency!=0){
		now = clock();
		if (now - wallb_start <= timer){
			timer -= (now - wallb_start);
			timer_disp = (float)timer/1000.0f;
			if (strcmp(hud_text, "**Felicitari! Ai gasit Iesirea.** Restart?(Y/N)")!=0){
				sprintf(hud_text, "Transparenta: %.3f", timer_disp);
			}
		}
		else {
			// A expirat timpul
			wallb_start = 0.0f;
			transparency = 1.0f;
			resetHudText();
		}
	}
}

void Game::countdownBlockDestruction(){
	clock_t timer = 15000.0f;
	clock_t now; 
	float timer_disp;
	if (destroyb_start!=0 && canDestroy){
		now = clock();
		if (now - destroyb_start <=timer){
			timer -= (now-destroyb_start);
			timer_disp = (float)timer/1000.0f;
			if (strcmp(hud_text, "**Felicitari! Ai gasit Iesirea.** Restart?(Y/N)")!=0){
				sprintf(hud_text, "Spargere cuburi: %.3f", timer_disp);
			}
		}
		else {
			// A expirat timpul
			destroyb_start = 0;
			canDestroy = false;
			resetHudText();
		}
	}

}

