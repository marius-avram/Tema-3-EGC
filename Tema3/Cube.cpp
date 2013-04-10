#include "Cube.h" 
#include <cmath>

Cube::Cube(float scale, Vector3D position, float *transparency) : 
	Object(scale, position),
	transparency(transparency) {
}

Cube::~Cube(){
}

void Cube::draw(){
	// Cubul va fi desenat cu coltul stanga-jos-spate in orgine.
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
		glPushMatrix();
			glTranslatef(scale/2, scale/2, scale/2);
			glColor4f(0.0f, 0.0f, 0.0f, *transparency);
			glLineWidth(4.0f);
			glutWireCube(scale);
		glPopMatrix();
	glBegin(GL_QUADS);
		glColor4f(0.12f, 0.06f, 0.04f, *transparency);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, scale, 0.0f);
		glVertex3f(scale, scale, 0.0f);
		glVertex3f(scale, 0, 0.0f);
		
		glColor4f(0.11f, 0.05f, 0.03f, *transparency);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, scale);
		glVertex3f(0.0f, scale, scale); 
		glVertex3f(0.0f, scale, 0.0f);

		glColor4f(0.11f, 0.05f, 0.03f, *transparency);
		glVertex3f(scale, 0.0f, 0.0f);
		glVertex3f(scale, 0.0f, scale); 
		glVertex3f(scale, scale, scale);
		glVertex3f(scale, scale, 0.0f);
		
		glColor4f(0.09f, 0.03f, 0.02f, *transparency);
		glVertex3f(0.0f, 0.0f, scale); 
		glVertex3f(0.0f, scale, scale); 
		glVertex3f(scale, scale, scale);
		glVertex3f(scale, 0.0f, scale);
		
		// Fata de sus 
		glColor4f(0.2f, 0.09f, 0.07f, *transparency);
		glVertex3f(0.0f, scale, 0.0f); 
		glVertex3f(0.0f, scale, scale); 
		glVertex3f(scale, scale, scale); 
		glVertex3f(scale, scale, 0.0f);
		
		// Fata de jos
		glColor4f(0.2f, 0.09f, 0.07f, *transparency);
		glVertex3f(0.0f, 0.0f, 0.0f); 
		glVertex3f(0.0f, 0.0f, scale);
		glVertex3f(scale, 0.0f, scale); 
		glVertex3f(scale, 0.0f, 0.0f);
	glEnd();
	glPopMatrix();

	// Desenam caramizile
	glLineWidth(2.0f);
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z-0.01f);
		drawBricksModel();
	glPopMatrix();
	glPushMatrix(); 
		glTranslatef(position.x, position.y, position.z+scale+0.01f);
		drawBricksModel();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(position.x-0.01f, position.y, position.z+scale);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		drawBricksModel();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(position.x+scale+0.01f, position.y, position.z+scale);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		drawBricksModel();
	glPopMatrix();
}


void Cube::drawBricksModel(){
	float distance = scale/3;
	glColor4f(0.01f, 0.01f, 0.01f, *transparency);
	// Aici avem o singura caramida
	glPushMatrix();
	for(int i=0; i<scale; i++){
		glPushMatrix();
		for(int j=0; j<(scale+1)/2; j++){
			glBegin(GL_LINES);
				glVertex3f(0.0f, 0.0f, 0.0f); 
				glVertex3f(0.0f, 1.0f, 0.0f);
				glVertex3f(0.0f, 1.0f, 0.0f);
				glVertex3f(distance, 1.0f, 0.0f);
				glVertex3f(distance, 1.0f, 0.0f);
				glVertex3f(distance, 0.0f, 0.0f);
				glVertex3f(distance, 0.0f, 0.0f); 
				glVertex3f(0.0f, 0.0f, 0.0f);
			glEnd();
			glTranslatef(distance, 0.0f, 0.0f);
		}
		glPopMatrix();
		glTranslatef(0.0f, 1.0f, 0.0f);
	}
	glPopMatrix();
}
/** Verifica daca jucatorul este in interiorul cubului.
 *  Primeste ca parametru pozitia jucatorului si dimensiunea
 *  razei de coliziune pentru bounding sphere */ 
bool Cube::insideObject(Vector3D player_pos, float radius){

	float center1_x = position.x, center1_z = position.z;
	float center2_x = position.x + scale, 
		  center2_z = position.z + scale;
   
	float dist_squared = radius * radius;
	if (player_pos.x < center1_x){
		dist_squared -= pow(player_pos.x - center1_x,2);
	}
	else if (player_pos.x > center2_x){
		dist_squared -= pow(player_pos.x - center2_x,2);
	}
	if (player_pos.z < center1_z){
		dist_squared -= pow(player_pos.z - center1_z,2);
	}
	else if (player_pos.z > center2_z){
		dist_squared -= pow(player_pos.z - center2_z, 2);
	}
	float distance = sqrt(pow(player_pos.x + position.x+scale/2,2) + pow(player_pos.z + position.z+scale/2,2));

	return dist_squared > 0;
}

Vector3D Cube::getCenterPosition(){
	Vector3D center = position;
	center.x += scale/2;
	center.z += scale/2;
	return center;
}