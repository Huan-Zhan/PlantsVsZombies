#include "PlantNut.h"
#include <graphics.h>	

using namespace std;

IMAGE PlantNut::getPlantImageMove() {
	return PlantImageMove;
}

IMAGE PlantNut::getPlantImage() {
	return PlantImage;
}

MyPVZAction PlantNut::getPlantImageAction() {
	return PlantImageAction;
}
void PlantNut::setPlantImage() {
	loadimage(&PlantImage, "PlantsVsZombiesAllMaterial/res/Cards/card_wallnut.png");
	loadimage(&PlantImageMove, "PlantsVsZombiesAllMaterial/res/zhiwu/3/1.gif");

	char ImagePath[64];
	PlantImageAction.used = 13;
	for (int i = 0; i < 3; i++) {

		sprintf_s(ImagePath, sizeof(ImagePath), "PlantsVsZombiesAllMaterial/res/zhiwu/3/%d.gif", i + 1);

		loadimage(&PlantImageAction.Myaction[i], ImagePath);
	}


}

PlantNut::PlantNut() {
	PlantHp = 100;
	setPlantImage();
}