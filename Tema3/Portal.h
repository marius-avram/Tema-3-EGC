#pragma once 
#include "Vector3D.h"
#include "Object.h" 

class DynamicCamera;
class Portal : public Object{
	public:
		Portal(float size, float rotation, Vector3D position, DynamicCamera &camera);
		virtual ~Portal();
		bool insideObject(Vector3D player_pos, float player_radius);
		void draw();
	private:
		float rotation;
		float radius;
		DynamicCamera &camera;
};