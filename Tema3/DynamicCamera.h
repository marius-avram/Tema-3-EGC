#pragma once
#include "Vector3D.h"


class DynamicCamera{
	public:
		DynamicCamera();
		DynamicCamera(float tps_radius);
		~DynamicCamera();

		void translate_Forward(float dist);
		void translate_Up(float dist);
		void translate_Right(float dist);

		void rotateFPS_OY(float angle);
		void rotateFPS_OX(float angle);
		void rotateFPS_OZ(float angle);
		void rotateTPS_OY(float angle, float dist_to_interes);
		void rotateTPS_OX(float angle, float dist_to_interes);
		void rotateTPS_OZ(float angle, float dist_to_interes);
		void setRotationTPS_OX(float angle, float dist_to_interes);
		void setRotationTPS_OY(float angle, float dist_to_interes);
		void setRotationTPS_OZ(float angle, float dist_to_interes);
		void resetTPSCamera();
			
		void init();
		void render();
		void setTPSCamera();
		void setFPSCamera();
		void saveVectors();
		void restoreVectors();
		int getTPSRadius();
		bool isTPSCamera();
		void setPosition(float x, float y, float z);
		void setPosition(Vector3D position);
		Vector3D getPosition();
		Vector3D getForward();

	private:
		Vector3D forward, up, right, position;
		Vector3D forward_copy, up_copy, right_copy, position_copy;
		bool setTPS;
		float tps_radius;
		float tps_ox_angle, tps_oy_angle, tps_oz_angle;
		float tps_dist_to_interes;
};