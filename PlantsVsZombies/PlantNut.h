#pragma once
#include <graphics.h>	
#include"MyPVZAction.h"
	using namespace std;
	class PlantNut {
	public:

		IMAGE PlantImage, PlantImageMove;
		int PlantHp;
		MyPVZAction PlantImageAction;
		PlantNut();
		IMAGE getPlantImage();
		IMAGE getPlantImageMove();
		MyPVZAction getPlantImageAction();
		void setPlantImage();
	};


