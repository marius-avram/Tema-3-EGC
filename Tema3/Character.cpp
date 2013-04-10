#include "Character.h"
#include "DynamicCamera.h"
#include "Game.h"

Character::Character(DynamicCamera &camera, float *transparency): 
	camera(camera),
	transparency(transparency) {
}

Character::~Character(){
}

void Character::draw(){
	float color4f[] = {0.0f, 1.0f, 0.0f, *transparency};
	updatePosition();
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color4f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color4f);
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -0.5f);
		glutSolidCone(1.0f, 2.0f, 15, 15);
		glPushMatrix();
			glTranslatef(0.0f, 0.1f, -0.5f);
			glutSolidSphere(0.4f, 15, 15);
		glPopMatrix();
	glPopMatrix();
	glDisable(GL_LIGHTING);
	
}

void Character::updatePosition(){
	position = camera.getPosition();
	if (Game::getCameraMode() == 2){
		camera.translate_Forward(camera.getTPSRadius());
		position = camera.getPosition();
		camera.translate_Forward(-camera.getTPSRadius());
	}
}

bool Character::checkCollision(Cube &cube){
	return cube.insideObject(position, 1.3f);
}


