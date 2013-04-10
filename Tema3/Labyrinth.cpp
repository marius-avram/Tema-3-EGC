#include "Labyrinth.h"
#include "DynamicCamera.h"
#include "Cube.h" 
#include "Ground.h"
#include "Vector3D.h"
#include <iostream>
#include <ctime> 
#define _USE_MATH_DEFINES
#include <math.h> 

float Labyrinth::CUBE_SCALE = 5.0f;

Labyrinth::Labyrinth(DynamicCamera &camera, float *transparency) : 
	camera(camera), 
	transparency(transparency) {
	player_i = 0; player_j = 0;
	portal_i = 0; portal_j = 0;
	wallb_i = 0; wallb_j = 0;
	destroyb_i = 0; destroyb_j = 0;
	last_collided_i = -1; 
	last_collided_j = -1;
	camera_rotation = 0;
	portal_rotation = 0;
	srand((unsigned int)time(NULL));
	readMapFromFile();
	calculateCameraPosition();
	calculateCameraRadRotation();
	calculatePortalPosition();
	calculatePortalRotation();
	calculateWallBonusPosition();
	calculateDestroyBonusPosition();
	init();
}

Labyrinth::~Labyrinth(){

	for (unsigned int i=0; i<cubes.size(); ++i){
		for (unsigned int j=0; j<cubes[i].size(); j++){
			delete cubes[i][j];
		}
	}
	for (unsigned int i=0; i<ground_tiles.size(); ++i){
		for (unsigned int j=0; j<ground_tiles[i].size(); ++j){
			delete ground_tiles[i][j];
		}
	}
}

void Labyrinth::readMapFromFile(){
	char character = ' ';
	char map_name[20];
	sprintf_s(map_name, "maps/labyrinth%i.map", rand() % 3 + 1); 
	std::ifstream file;
	file.open(map_name);
	if (file.is_open()){
		file >> lines >> columns;
		if (lines>50 || columns>50){
			std::cout << "Dimensiunea hartii trebuie sa fie de maxim 50x50." << std::endl;
			return;
		}
		for (int i=0; i<lines; i++){
			for (int j=0; j<columns; j++){
				file >> character;
				if (character=='0'){
					cubesLocations[i][j]=false;
				}
				else if (character=='1'){
					cubesLocations[i][j]=true;
				}
			}
		}
		
		generateRandomLocations(file);
	}
	else {
		std::cout << "Fisierul de map nu a putut fi deschis" << std::endl;
	}
	

	file.close();

}

// Am cam exagerat aici cu numarul variabilelor. Ar fi 
// trebuit sa fac un array de locatii.
void Labyrinth::generateRandomLocations(std::ifstream &file){
	int player_position_i, player_position_j, 
		portal_position_i, portal_position_j,
		wallb_position_i, wallb_position_j,
		destroyb_position_i, destroyb_position_j;
	clock_t start = clock();
	
	// Citim valorile de rezerva pentru pozitia jucatorului,
	// portalului si a bonusurilor
	file >> player_position_i >> player_position_j;
	file >> portal_position_i >> portal_position_j;
	file >> wallb_position_i >> wallb_position_j;
	file >> destroyb_position_i >> destroyb_position_j;

	// Verificam ca nu cumva valorile date sa nu depaseasca 
	// limitele dimensiunii labirintului
	if ((player_position_i < 0 || player_position_i >= lines) || 
		(player_position_j < 0 || player_position_j >= columns)){
			std::cout << "Pozitia jucatorului este invalida" << std::endl;
	}
	if ((portal_position_i < 0 || portal_position_i >= lines) || 
		(portal_position_j < 0 || portal_position_j >= columns)){
			std::cout << "Pozitia portalului este invalida" << std::endl;
	}
	if ((wallb_position_i < 0 || wallb_position_i >= lines) || 
		(wallb_position_j < 0 || wallb_position_j >= columns)){
			std::cout << "Pozitia wall bonus este invalida" << std::endl;
	}
	if ((destroyb_position_i < 0 || destroyb_position_i >= lines) || 
		(destroyb_position_j < 0 || destroyb_position_j >= columns)){
			std::cout << "Pozitia destoy bonus este invalida" << std::endl;
	}
	

	while(1){
		if (cubesLocations[player_i][player_j]){
			player_i = rand() % lines;
			player_j = rand() % columns;
		}
		
		if (cubesLocations[portal_i][portal_j]){
			portal_i = rand() % lines; 
			portal_j = rand() % columns;
		}

		if (cubesLocations[wallb_i][wallb_j]){
			wallb_i = rand() % lines; 
			wallb_j = rand() % columns;
		}

		if (cubesLocations[destroyb_i][destroyb_j]){
			destroyb_i = rand() % lines; 
			destroyb_j = rand() % columns;
		}

		if (!cubesLocations[player_i][player_j] && 
			!cubesLocations[portal_i][portal_j] && 
			!cubesLocations[wallb_i][wallb_j] && 
			!cubesLocations[destroyb_i][destroyb_j]){
			break;
		}


		// Daca nu se pot genera numere aleatoare bune pentru 
		// pozitia jucatorului, a portalului sau pentru bonusuri
		// se folosesc cele de rezerva din fisier.
		if ((clock() - start) > 1000){
			if (!cubesLocations[player_i][player_j]){
				player_i = player_position_i;
				player_j = player_position_i;
			}
			if (!cubesLocations[player_i][player_j]){
				portal_i = portal_position_i;
				portal_j = portal_position_j;
			}
			if (!cubesLocations[wallb_i][wallb_j]){
				wallb_i = wallb_position_i;
				wallb_j = wallb_position_j;
			}
			if (!cubesLocations[destroyb_i][destroyb_j]){
				destroyb_i = destroyb_position_i;
				destroyb_j = destroyb_position_j;
			}
			break;
		}
	}

	// Debug info
	//std::cout << "Player pos: " << player_i << " " << player_j;
	//std::cout << "Portal pos: " << portal_i << " " << portal_j;
}

void Labyrinth::init(){
	// Se initializeaza vectorul de vectori ce va contine cuburile
	Vector3D position(0.0f, 0.0f, 0.0f);
	for(int i=0; i<lines; i++){
		cubes.push_back(std::vector<Cube*>());
		ground_tiles.push_back(std::vector<Ground*>());
	}
	
	for(int i=lines-1; i>=0; i--){
		for(int j=0; j<columns; j++){
			// Se creaza un nou cub doar daca el exista pe harta
			if (cubesLocations[i][j]){
				cubes[i].push_back(new Cube(CUBE_SCALE, position, transparency));
			} else { 
				cubes[i].push_back(NULL);
			}
			ground_tiles[i].push_back(new Ground(CUBE_SCALE, position));
			position.x += CUBE_SCALE;
		}
		position.x -= CUBE_SCALE*(columns);
		position.z -= CUBE_SCALE;
	}

}

void Labyrinth::draw(){
	for(int i=0; i<lines; i++){
		for(int j=0; j<columns; j++){
			if (cubes[i][j] != NULL){
				cubes[i][j]->draw();
			}
			ground_tiles[i][j]->draw();
		}
	}
}

bool Labyrinth::isCollidingWall(float radius){	
	Vector3D position = camera.getPosition(), pos;
	int neighbours_i[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1},
		neighbours_j[] = { 0, 1, -1, 0, 1, -1, 0, 1, -1};
	int numb_neighbours = 9;
	int i=0, j=0;
	
	// Daca camera este in modul TPS atunci pozitia 
	// personajului este egala cu pozitia camerei in modul TPS 
	if (camera.isTPSCamera()){
		camera.setFPSCamera();
		position = camera.getPosition();
		camera.setTPSCamera();
	}
	pos = position;

	// Aflam pozitia jucatorului in labirint
	while(position.x > CUBE_SCALE){
		position.x -=CUBE_SCALE;
		j++;
	}
	while(position.z < 0){
		position.z += CUBE_SCALE;
		i++;
	}
	i = (lines-1)-i;
	
	// Verificam coliziunile doar cu cuburile din jur
	for(int index=0; index<numb_neighbours; index++){
		if (i+neighbours_i[index]<lines && i+neighbours_i[index]>=0 && 
			j+neighbours_j[index]<columns && j+neighbours_j[index]>=0){
			// Daca vecinul este un cub verificam coliziunea cu acesta
			if (cubes[i+neighbours_i[index]][j+neighbours_j[index]]!=NULL){
				if (cubes[i+neighbours_i[index]][j+neighbours_j[index]]->insideObject(pos, radius)){
					last_collided_i = i+neighbours_i[index];
					last_collided_j = j+neighbours_j[index];
					return true;
				}
			}
		}
	}
	return false;
}

void Labyrinth::destoryLastCollided(){
	float distance = 0.0f;
	Vector3D cube_pos, player_pos;
	if (last_collided_i>0 && last_collided_j>0 && 
		last_collided_i<lines-1 && last_collided_j<columns-1){
		// Verificam daca personajul se afla in vecinatatea cubului.
		// Doar atunci va fi distrus. Se face acest lucru pentru a
		// imita folosirea unei unelte ce distruge cuburile si ce 
		// are doar o anumita raza de actiune.
		player_pos = camera.getPosition();
		if (camera.isTPSCamera()){
			camera.setFPSCamera();
			player_pos = camera.getPosition();
			camera.setTPSCamera();
		}
		cube_pos = cubes[last_collided_i][last_collided_j]->getCenterPosition();
		distance = sqrt(pow(cube_pos.x-player_pos.x,2)+pow(cube_pos.z-player_pos.z,2));
		if (distance < CUBE_SCALE){
			delete cubes[last_collided_i][last_collided_j];
			cubes[last_collided_i][last_collided_j] = NULL;
			last_collided_i = -1; 
			last_collided_j = -1;
		}
	}
}

void Labyrinth::calculateCameraPosition(){
	camera_position = Vector3D(0.0f, 4.0f, 0.0f);
	camera_position.x = CUBE_SCALE*(player_j)+CUBE_SCALE/2;
	camera_position.z = -CUBE_SCALE*(lines-(player_i+2))-CUBE_SCALE/2;
}

void Labyrinth::calculatePortalPosition(){
	portal_position = Vector3D(0.0f, 2.4f, 0.0f);
	portal_position.x = CUBE_SCALE*portal_j+CUBE_SCALE/2;
	portal_position.z = -CUBE_SCALE*(lines-(portal_i+2))-CUBE_SCALE/2;
}

void Labyrinth::calculateWallBonusPosition(){
	wallb_position = Vector3D(0.0f, 2.4f, 0.0f);
	wallb_position.x = CUBE_SCALE*wallb_j+CUBE_SCALE/2;
	wallb_position.z = -CUBE_SCALE*(lines-(wallb_i+2))-CUBE_SCALE/2;
}

void Labyrinth::calculateDestroyBonusPosition(){
	destroyb_position = Vector3D(0.0f, 2.4f, 0.0f);
	destroyb_position.x = CUBE_SCALE*destroyb_j+CUBE_SCALE/2;
	destroyb_position.z = -CUBE_SCALE*(lines-(destroyb_i+2))-CUBE_SCALE/2;
}


void Labyrinth::calculatePortalRotation(){
	portal_rotation = 0.0f;
	if ((portal_i>0 && portal_i<lines) && (cubesLocations[portal_i+1][portal_j] 
		|| cubesLocations[portal_i-1][portal_j])){
		portal_rotation = 90.0f;
	}
	else if (portal_i==0 && cubesLocations[portal_i+1][portal_j]){
		portal_rotation = 90.0f;
	}
	else if (portal_i==lines && cubesLocations[portal_i-1][portal_j]){
		portal_rotation = 90.0f;
	}
}

void Labyrinth::calculateCameraRadRotation(){
	camera_rotation = 0.0f;
	if (player_i != 0){
		// Daca este cub fix inainte il rotim 90 de grade 
		if (cubesLocations[player_i-1][player_j]){
			camera_rotation += M_PI/2;

			if (player_j!=columns){
				// Daca dupa rotatie tot aveam cub in fata 
				// il rotim in continuare
				if (cubesLocations[player_i][player_j+1]){
					camera_rotation+= M_PI/2;

					// La fel si pentru ultima rotatie
					if (player_i!=lines){
						if (cubesLocations[player_i+1][player_j]){
							camera_rotation+= M_PI/2;
						}
					}
				}
			}
		}
	}
}

Vector3D Labyrinth::getCameraPosition(){
	return camera_position; 
}

Vector3D Labyrinth::getPortalPosition(){
	return portal_position;
}

Vector3D Labyrinth::getWallBonusPosition(){
	return wallb_position;
}

Vector3D Labyrinth::getDestroyBonusPosition(){
	return destroyb_position;
}

float Labyrinth::getCameraRadRotation(){
	return camera_rotation;
}
float Labyrinth::getPortalRotation(){
	return portal_rotation;
}

