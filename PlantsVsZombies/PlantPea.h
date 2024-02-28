#pragma once
#include <graphics.h>
#include"MyPVZAction.h"
using namespace std;

class PlantPea {
public:

	IMAGE PlantImage, PlantImageMove; //豌豆射手的状态栏图片 和 跟随鼠标移动时的图片
	int PlantHp;	// 豌豆射手的血量
	MyPVZAction PlantImageAction; // 豌豆射手的状态
	int costSun ;

	PlantPea();// 豌豆射手的无参构造器
	IMAGE getPlantImage();	// 获得豌豆射手的豌豆射手的状态栏图片
	IMAGE getPlantImageMove(); //获得豌豆射手跟随鼠标移动时的图片
	MyPVZAction getPlantImageStateAction(); // 获得豌豆射手的动作画面
	
	void setPlantImage();
};
