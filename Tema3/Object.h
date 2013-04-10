#pragma once 
#include "Vector3D.h" 

class Object{
	public:
		Object();
		Object(float scale, Vector3D position){
			this->scale = scale;
			this->position = position;
		}
		virtual ~Object(){}
		virtual bool insideObject(Vector3D player_pos, float radius) = 0;
		virtual void draw() = 0;
	protected:
		float scale; 
		Vector3D position;
};