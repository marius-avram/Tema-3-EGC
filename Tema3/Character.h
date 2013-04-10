#pragma once
#include "Vector3D.h"
#include "Cube.h"

class DynamicCamera;
class Character {
	public:
		Character(DynamicCamera &camera, float *transparency);
		virtual ~Character();
		void updatePosition();
		bool checkCollision(Cube& cube);
		void draw();
	private:
		DynamicCamera &camera;
		Vector3D position;
		float *transparency;
};