#include"PlantPea.h"
#include <graphics.h>	

using namespace std;

IMAGE PlantPea::getPlantImageMove() {
	return PlantImageMove;
}

IMAGE PlantPea::getPlantImage() {
	return PlantImage;
}

MyPVZAction PlantPea::getPlantImageStateAction() {
	return PlantImageAction;
}
void PlantPea::setPlantImage() {
	loadimage(&PlantImage, "PlantsVsZombiesAllMaterial/res/Cards/card_1.png");
	loadimage(&PlantImageMove, "PlantsVsZombiesAllMaterial/res/zhiwu/0/1.png");

	char ImagePath[64];
	PlantImageAction.used = 13;
	for (int i = 0; i < 13; i++) {

		sprintf_s(ImagePath,sizeof(ImagePath),"PlantsVsZombiesAllMaterial/res/zhiwu/0/%d.png",i+1);

		loadimage(&PlantImageAction.Myaction[i],ImagePath);
	}

	/*loadimage(&PlantImageAction[0], "PlantsVsZombiesAllMaterial/res/zhiwu/0/1.png");
	loadimage(&PlantImageAction[1], "PlantsVsZombiesAllMaterial/res/zhiwu/0/2.png");
	loadimage(&PlantImageAction[2], "PlantsVsZombiesAllMaterial/res/zhiwu/0/3.png");
	loadimage(&PlantImageAction[3], "PlantsVsZombiesAllMaterial/res/zhiwu/0/4.png");
	loadimage(&PlantImageAction[4], "PlantsVsZombiesAllMaterial/res/zhiwu/0/5.png");
	loadimage(&PlantImageAction[5], "PlantsVsZombiesAllMaterial/res/zhiwu/0/6.png");
	loadimage(&PlantImageAction[6], "PlantsVsZombiesAllMaterial/res/zhiwu/0/7.png");
	loadimage(&PlantImageAction[7], "PlantsVsZombiesAllMaterial/res/zhiwu/0/8.png");
	loadimage(&PlantImageAction[8], "PlantsVsZombiesAllMaterial/res/zhiwu/0/9.png");
	loadimage(&PlantImageAction[9], "PlantsVsZombiesAllMaterial/res/zhiwu/0/10.png");
	loadimage(&PlantImageAction[10], "PlantsVsZombiesAllMaterial/res/zhiwu/0/11.png");
	loadimage(&PlantImageAction[11], "PlantsVsZombiesAllMaterial/res/zhiwu/0/12.png");
	loadimage(&PlantImageAction[12], "PlantsVsZombiesAllMaterial/res/zhiwu/0/13.png");*/
}

PlantPea::PlantPea(){
	
	costSun = 100;
	PlantHp = 10;
	setPlantImage();
}

