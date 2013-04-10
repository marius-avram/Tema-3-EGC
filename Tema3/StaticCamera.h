#pragma once 
#include "Vector3D.h"

class StaticCamera{
	public:
		StaticCamera();
		virtual ~StaticCamera();
		void init();
		void render();
	private:
		Vector3D position, center, up;
};