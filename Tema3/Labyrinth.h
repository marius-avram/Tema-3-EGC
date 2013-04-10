#pragma once 
#include <iostream>
#include <fstream>
#include <vector> 
#include "Vector3D.h"

class Cube;
class Ground;
class DynamicCamera;
class Labyrinth{
	public:
		Labyrinth(DynamicCamera &camera, float *transparency);
		virtual ~Labyrinth();
		void readMapFromFile();
		void generateRandomLocations(std::ifstream &file);
		void init();
		void draw();
		bool isCollidingWall(float radius);
		void destoryLastCollided();

	private:
		void calculateCameraPosition();
		void calculateCameraRadRotation();
		void calculatePortalPosition();
		void calculatePortalRotation();
		void calculateWallBonusPosition();
		void calculateDestroyBonusPosition();

	public:
		Vector3D getCameraPosition();
		Vector3D getPortalPosition();
		float getCameraRadRotation();
		float getPortalRotation();
		Vector3D getWallBonusPosition();
		Vector3D getDestroyBonusPosition();

	private:
		DynamicCamera &camera;
		int lines, columns;
		bool cubesLocations[50][50];
		int player_i, player_j, portal_i, portal_j, wallb_i, wallb_j,
			destroyb_i, destroyb_j;
		// Retine ultimul cub cu care s-a facut coliziune
		int last_collided_i, last_collided_j;
		std::vector<std::vector<Cube*>> cubes;
		std::vector<std::vector<Ground*>> ground_tiles;
		Vector3D camera_position, portal_position, wallb_position, destroyb_position;
		float camera_rotation, portal_rotation;
		static float CUBE_SCALE;
		float *transparency;
};