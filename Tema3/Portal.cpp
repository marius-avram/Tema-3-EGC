#include "Portal.h" 
#include "DynamicCamera.h"
#include <cmath> 


Portal::Portal(float scale, float rotation, Vector3D position, DynamicCamera &camera): 
		Object(scale, position), rotation(rotation), camera(camera) {
	radius = 1.2f;
}

Portal::~Portal(){
}

bool Portal::insideObject(Vector3D player_pos, float player_radius){
	if (camera.isTPSCamera()){
		camera.setFPSCamera();
		player_pos = camera.getPosition();
		camera.setTPSCamera();
	}
	float distance; 
	// Calculam distanta intre jucator si portal doar 
	// tinan seama de coordonatele x si z
	distance = sqrt(pow(player_pos.x-position.x, 2)+pow(player_pos.z-position.z,2));
	return (distance < radius+player_radius+2.0f);
}

void Portal::draw(){
	float red4f[] = {1.0f, 0.0f, 0.0f, 1.0f};
	float yellow4f[] = {1.0f, 1.0f, 0.0f, 1.0f};
	glEnable(GL_LIGHTING);
	glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red4f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red4f);
		glTranslatef(position.x, position.y, position.z);
		glRotatef(rotation, 0.0f, 1.0f, 0.0f);
		glutSolidTorus(0.5f, radius, 7, 20);
	glPopMatrix();
	glDisable(GL_LIGHTING);
}



