#include "DynamicCamera.h"
#define _USE_MATH_DEFINES
#include <math.h> 

DynamicCamera::DynamicCamera(){}


DynamicCamera::DynamicCamera(float tps_radius){
	this->tps_radius = tps_radius;
	tps_ox_angle = M_PI/5; 
	tps_oz_angle = 0.0f;
	tps_oy_angle = 0.0f;
	tps_dist_to_interes = 10.0f;
	setTPS = false; 
}

DynamicCamera::~DynamicCamera(){
}

void DynamicCamera::init(){
	position = Vector3D(0,4,0);
	forward = Vector3D(0,0,-1);
	up = Vector3D(0,1,0);
	right = Vector3D(1,0,0);
}

void DynamicCamera::translate_Forward(float dist){
	position += forward * dist;
}
void DynamicCamera::translate_Up(float dist){
	position += up * dist;
}
void DynamicCamera::translate_Right(float dist){
	position += right * dist;
}

void DynamicCamera::setTPSCamera(){
	if (!setTPS){
		translate_Forward(-tps_radius);
		setTPS = true;
	}
}

void DynamicCamera::setFPSCamera(){
	if (setTPS){
		translate_Forward(tps_radius);
		setTPS = false;
	}
}

bool DynamicCamera::isTPSCamera(){
	return setTPS;
}

Vector3D DynamicCamera::getPosition(){
	return position;
}

Vector3D DynamicCamera::getForward(){
	return forward;
}

int DynamicCamera::getTPSRadius(){
	return tps_radius;
}

void DynamicCamera::setPosition(float x, float y, float z){
	position = Vector3D(x, y, z);
}

void DynamicCamera::setPosition(Vector3D position){
	this->position = position;
}

void DynamicCamera::saveVectors(){
	forward_copy = forward;
	right_copy = right; 
	up_copy = up;
	position_copy = position;
}
void DynamicCamera::restoreVectors(){
	forward = forward_copy;
	right = right_copy; 
	up = up_copy; 
	position = position_copy;
}

void DynamicCamera::rotateFPS_OY(float angle){
	forward = forward * cos(angle) + right * sin(angle);
	right = forward.CrossProduct(up);
}
void DynamicCamera::rotateFPS_OX(float angle){
	up = up * cos(angle) + forward * sin(angle);
	forward = up.CrossProduct(right);
}
void DynamicCamera::rotateFPS_OZ(float angle){
	right = right * cos(angle) + up * sin(angle);
	up = right.CrossProduct(forward);
}
void DynamicCamera::rotateTPS_OY(float angle, float dist_to_interes){
	Vector3D oldForward = forward;
	forward = forward * cos(angle) + right * sin(angle);
	right = forward.CrossProduct(up);
	position = position + oldForward * dist_to_interes - forward * dist_to_interes;
}
void DynamicCamera::rotateTPS_OX(float angle, float dist_to_interes){
	Vector3D oldForward = forward;
	up = up * cos(angle) + forward * sin(angle);
	forward = up.CrossProduct(right);
	position = position + oldForward * dist_to_interes - forward * dist_to_interes;
}
void DynamicCamera::rotateTPS_OZ(float angle, float dist_to_interes){
	Vector3D oldForward = forward;
	right = right * cos(angle) + up * sin(angle);
	up = right.CrossProduct(forward);
	position = position + oldForward * dist_to_interes - forward * dist_to_interes;
}

void DynamicCamera::setRotationTPS_OX(float angle, float dist_to_interes){
	if (tps_ox_angle+angle>0  && tps_ox_angle+angle<M_PI/3){
		tps_ox_angle += angle;
	}
	tps_dist_to_interes = dist_to_interes;
}

void DynamicCamera::setRotationTPS_OY(float angle, float dist_to_interes){
	if (tps_oy_angle+angle>-M_PI/5 && tps_oy_angle+angle<M_PI/5){
		tps_oy_angle += angle;
	}
	tps_dist_to_interes = dist_to_interes;
}

void DynamicCamera::setRotationTPS_OZ(float angle, float dist_to_interes){
	if (tps_oz_angle+angle>-M_PI/4 && tps_oz_angle+angle<M_PI/4){
		tps_oz_angle += angle;
	}
	tps_dist_to_interes = dist_to_interes;
}

void DynamicCamera::resetTPSCamera(){
	tps_ox_angle = M_PI/5;
	tps_oy_angle = 0.0f;
	tps_oz_angle = 0.0f;
}

void DynamicCamera::render(){
	Vector3D center = position + forward;
	if (setTPS){
		saveVectors();
		if (tps_ox_angle!=0){
			rotateTPS_OX(tps_ox_angle, tps_dist_to_interes);
			center = position + forward;
		}
		if (tps_oz_angle!=0){
			rotateTPS_OZ(tps_oz_angle, tps_dist_to_interes);
			center = position + forward;
		}
		if (tps_oy_angle!=0){
			rotateTPS_OY(tps_oy_angle, tps_dist_to_interes); 
			center = position + forward;
		}
		gluLookAt(	position.x, position.y, position.z, 
					center.x, center.y, center.z,
					up.x, up.y, up.z);
		if (tps_ox_angle!=0 || tps_oy_angle!=0 || tps_oz_angle!=0){
			restoreVectors();
		}
	}
	else { 
		gluLookAt( position.x, position.y-2.0f, position.z, 
				   center.x, center.y-2.0f, center.z, 
				   up.x, up.y, up.z);
	}
}