#include "StaticCamera.h" 

StaticCamera::StaticCamera(){
}

StaticCamera::~StaticCamera(){
}

void StaticCamera::init(){
	position = Vector3D(50.0f, 148.0f, -50.0f);
	center   = Vector3D(50.0f, 0.0f, -50.0f);
	up		 = Vector3D(0.0f, 0.0f, -1.0f);
}


void StaticCamera::render(){
	gluLookAt(position.x, position.y, position.z,
			  center.x, center.y, center.z, 
			  up.x, up.y, up.z);
}