#include "Plantsunflower.h"
#include <graphics.h>	

using namespace std;

IMAGE PlantSunflower::getPlantImageMove() {
	return PlantImageMove;
}

IMAGE PlantSunflower::getPlantImage() {
	return PlantImage;
}

MyPVZAction PlantSunflower::getPlantImageAction() {
	return PlantImageAction;
}
void PlantSunflower::setPlantImage() {
	loadimage(&PlantImage, "PlantsVsZombiesAllMaterial/res/Cards/card_2.png");
	loadimage(&PlantImageMove, "PlantsVsZombiesAllMaterial/res/zhiwu/1/1.png");

	char ImagePath[64];
	PlantImageAction.used = 13;
	for (int i = 0; i < 13; i++) {

		sprintf_s(ImagePath, sizeof(ImagePath), "PlantsVsZombiesAllMaterial/res/zhiwu/1/%d.png", i + 1);

		loadimage(&PlantImageAction.Myaction[i], ImagePath);
	}

	
}

PlantSunflower::PlantSunflower() {
	PlantHp = 10;
	setPlantImage();
}