#include"PeaGrains.h"
PeaGrains::PeaGrains() {
	index = 0;
	indey = 0;
	originx = 0;
	originy = 0;
	loadimage(&grains,"PlantsVsZombiesAllMaterial/res/zhiwu/0/14.png");
	loadimage(&boomgrains, "PlantsVsZombiesAllMaterial/res/zhiwu/0/15.png");
}
