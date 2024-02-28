#pragma once
#include <graphics.h>	
#include"MyPVZAction.h"
using namespace std;

class PlantSunflower {
public:

	IMAGE PlantImage, PlantImageMove;
	int PlantHp;
	MyPVZAction PlantImageAction;

	PlantSunflower();
	IMAGE getPlantImage();
	IMAGE getPlantImageMove();
	MyPVZAction getPlantImageAction();
	void setPlantImage();
};
