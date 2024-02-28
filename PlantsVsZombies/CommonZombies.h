
#pragma once
#include <graphics.h>	
#include"MyPVZAction.h"
using namespace std;

class CommonZombies {
public:
	int ZombieHp , survive , indey , index;
	MyPVZAction ZombieWalkAction , ZombieAttackAction , ZombieStandAction , ZombieDeadAction;

	CommonZombies();

	MyPVZAction getZombieWalkAction();// 见名知意
	MyPVZAction getZombieAttackAction();
	MyPVZAction getZombieStandAction();
	MyPVZAction getZombieDeadAction();
	void setZombieAction();
};
