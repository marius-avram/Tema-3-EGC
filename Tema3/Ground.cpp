#include "Ground.h" 

Ground::Ground(float scale, Vector3D position){
	this->scale = scale;
	this->position = position;
}

Ground::Ground(){
}

Ground::~Ground(){
}

void Ground::draw(){
	glPushMatrix();
		glTranslatef(position.x, position.y-0.1f, position.z);
		glBegin(GL_TRIANGLES);
			glColor3f(0.54f, 0.54f, 0.53f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(scale, 0.0f, 0.0f);
			glVertex3f(scale, 0.0f, scale);
			glColor3f(0.48f, 0.47f, 0.46f);
			glVertex3f(scale, 0.0f, scale);
			glVertex3f(0.0f, 0.0f, scale);
			glVertex3f(0.0f, 0.0f, 0.0f);
		glEnd();
		glLineWidth(4.0f);
		glColor3f(0.2f, 0.2f, 0.2f);
	glPopMatrix();
}