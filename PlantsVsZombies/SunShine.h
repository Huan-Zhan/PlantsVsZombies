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
	int index, indey; // �����λ�� 
	int aimY;// ����λ�õ�Y
	int Suntimer;
};
