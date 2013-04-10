#pragma once 
#include <ctime>
#include "DynamicCamera.h"
#include "StaticCamera.h"

class Labyrinth;
class Character;
class Portal;
class Bonus;
class Game{
	public:
		static void init();
		static void freeResources();
		static void display();
		static void reshape(int width, int height);
		static void idle();
		static void keyboard(unsigned char ch, int x, int y);
		static void mouseKey(int button, int state, int x, int y);
		static void mouseMovement(int x, int y);
		static int getCameraMode();
		static void printText(float x, float y, char *text);
		static void setHudText(char *text);
		static void resetHudText();
		static void displayHud();
		static void isgameOver();
		static void checkBonusPick();
		static void countdownTransparency();
		static void countdownBlockDestruction();

		static DynamicCamera dynamic_camera;
		static StaticCamera static_camera;
		static Character *character;
		static Portal *portal;
		static Bonus *wall_bonus, *destroy_bonus;
		static float total_camera_angleY;
		static float MOUSE_SPEED, MOVING_SPEED, 
					 TPS_CAMERA_RADIUS, COLLISION_RADIUS;
		static int offsetx, offsety, width, height;
		static float forward_character_pos, right_character_pos;
		static int camera_mode;
		static Labyrinth *labyrinth;
		static char hud_text[60];
		static bool canRestart;
		static float transparency, char_transparency;
		static bool canDestroy;
		static clock_t wallb_start, destroyb_start;
};