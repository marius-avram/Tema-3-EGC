#pragma once
#include "Vector3D.h"
#include "Object.h" 

class DynamicCamera;
class Bonus : public Object{
	public:
		Bonus(float size, Vector3D position, DynamicCamera &camera);
		virtual ~Bonus();
		void setColor(float red, float green, float blue);
		bool insideObject(Vector3D player_pos, float player_radius);
		void draw();
	private:
		DynamicCamera &camera;
	protected:
		float red, green, blue;
};