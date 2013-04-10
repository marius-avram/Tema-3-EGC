#include "Bonus.h" 
#include "DynamicCamera.h"

Bonus::Bonus(float size, Vector3D position, DynamicCamera &camera): 
	Object(size, position), 
	camera(camera) {
	red = 0.85f;
	green = 0.35f;
	blue = 0.0f;
}


Bonus::~Bonus(){
}

void Bonus::setColor(float red, float green, float blue){
	this->red = red; 
	this->green = green; 
	this->blue = blue;
}

bool Bonus::insideObject(Vector3D player_pos, float player_radius){
	if (camera.isTPSCamera()){
		camera.setFPSCamera();
		player_pos = camera.getPosition();
		camera.setTPSCamera();
	}
	float distance; 
	// Calculam distanta intre jucator si portal doar 
	// tinan seama de coordonatele x si z
	distance = sqrt(pow(player_pos.x-position.x, 2)+pow(player_pos.z-position.z,2));
	return (distance < scale+player_radius);
}

void Bonus::draw(){
	float orange4f[] = {red, green, blue, 1.0f};
	glEnable(GL_LIGHTING);
	glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, orange4f);
		glTranslatef(position.x, position.y, position.z);
		glScalef(scale, scale, scale);
		glutSolidDodecahedron();
	glPopMatrix();
	glDisable(GL_LIGHTING);
}