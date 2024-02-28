
#include"SunShine.h"

using namespace std;

MyPVZAction SunShine::getSunAction() {
	return MySunaction;
}

void SunShine::setSunAction() {
	/*
	* 写入阳光
	*/
	char ImagePath[64];
	MySunaction.used = 29;
	for (int i = 0; i < MySunaction.used; i++) {

		sprintf_s(ImagePath, sizeof(ImagePath), "PlantsVsZombiesAllMaterial/res/sunshine/%d.png", i + 1);

		loadimage(&MySunaction.Myaction[i], ImagePath);
	}
}

SunShine::SunShine() {
	index = 0 ;
	indey = 0 ; // 阳光的位置 
	aimY = 0;// 最终位置的Y
	Suntimer = 0;
	setSunAction();
}