#pragma once
#include <graphics.h>	
#include"MyPVZAction.h"
using namespace std;

class SunShine {
public :
	SunShine();
	MyPVZAction  MySunaction;
	MyPVZAction getSunAction();
	void setSunAction();
	int index, indey; // 阳光的位置 
	int aimY;// 最终位置的Y
	int Suntimer;
};
