#pragma once
#include "Object.h" 

class Cube : public Object{
	public:
		Cube(float scale, Vector3D position, float *transparency);
		virtual ~Cube();
		void draw();
		void drawBricksModel();
		bool insideObject(Vector3D player_pos, float radius);
		Vector3D getCenterPosition();
	private:
		float *transparency;
};