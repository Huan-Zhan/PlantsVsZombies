
#pragma once
#include <graphics.h>	
#include"MyPVZAction.h"
using namespace std;

class CommonZombies {
public:
	int ZombieHp , survive , indey , index;
	MyPVZAction ZombieWalkAction , ZombieAttackAction , ZombieStandAction , ZombieDeadAction;

	CommonZombies();

	MyPVZAction getZombieWalkAction();// ����֪��
	MyPVZAction getZombieAttackAction();
	MyPVZAction getZombieStandAction();
	MyPVZAction getZombieDeadAction();
	void setZombieAction();
};
