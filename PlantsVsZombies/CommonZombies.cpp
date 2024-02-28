
#include"CommonZombies.h"
#include <graphics.h>

using namespace std;

MyPVZAction CommonZombies::getZombieWalkAction() {
	return ZombieWalkAction;
}

MyPVZAction  CommonZombies::getZombieAttackAction() {
	return ZombieAttackAction;
}

MyPVZAction  CommonZombies::getZombieStandAction() {
	return ZombieStandAction;
}

MyPVZAction  CommonZombies::getZombieDeadAction() {
	return ZombieDeadAction;
}


void CommonZombies::setZombieAction() {

	char ImagePath[64];

	/*
	* 写入僵尸站立动作
	*/
	ZombieStandAction.used = 11;
	for (int i = 0; i < ZombieStandAction.used; i++) {

		sprintf_s(ImagePath, sizeof(ImagePath), "PlantsVsZombiesAllMaterial/res/zm_stand/%d.png", i + 1);

		loadimage(&ZombieStandAction.Myaction[i], ImagePath);
	}
	/*
	* 写入僵尸攻击动作
	*/
	ZombieAttackAction.used = 21;
	for (int i = 0; i < ZombieAttackAction.used; i++) {
		
		sprintf_s(ImagePath, sizeof(ImagePath), "PlantsVsZombiesAllMaterial/res/zm_eat/%d.png", i + 1);

		loadimage(&ZombieAttackAction.Myaction[i], ImagePath);
	}

	/*
	* 写入僵尸死亡动作
	*/
	ZombieDeadAction.used = 20;
	for (int i = 0; i < ZombieDeadAction.used; i++) {
		sprintf_s(ImagePath, sizeof(ImagePath), "PlantsVsZombiesAllMaterial/res/zm_dead/%d.png", i + 1);
		loadimage(&ZombieDeadAction.Myaction[i], ImagePath);
	}

	/*
	* 写入僵尸行走动作
	*/
	ZombieWalkAction.used = 22;
	for (int i = 0; i < ZombieWalkAction.used; i++) {

		sprintf_s(ImagePath, sizeof(ImagePath), "PlantsVsZombiesAllMaterial/res/zm/%d.png", i + 1);

		loadimage(&ZombieWalkAction.Myaction[i], ImagePath);
	}
}

CommonZombies::CommonZombies() {
	indey = 0;
	indey = 0;
	survive = 0;
	ZombieHp = 100;
	setZombieAction();
}


