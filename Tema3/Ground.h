#pragma once
#include <cmath>
#include "Vector3D.h"

class Ground{
	public:
		Ground(float scale, Vector3D position);
		Ground();
		virtual ~Ground();
		void draw();
	private:
		float scale; 
		Vector3D position;

};